#include "Map3D.h"
#include "ScalingEngine.h"
#include "binary/Endianness.h"
#include <cstring>
#include <algorithm>

namespace WinMMM10 {

Map3D::Map3D() = default;

Map3D::Map3D(const MapDefinition& definition) : m_definition(definition) {
    m_rows = definition.rows();
    m_columns = definition.columns();
    m_data.resize(m_rows * m_columns);
    
    if (definition.hasXAxis()) {
        m_xAxis.resize(definition.xAxis().count());
    }
    if (definition.hasYAxis()) {
        m_yAxis.resize(definition.yAxis().count());
    }
}

void Map3D::loadFromBinary(const uint8_t* data, size_t dataSize) {
    if (!data || dataSize == 0) {
        return;
    }
    
    size_t offset = 0;
    
    // Load X axis if present
    if (m_definition.hasXAxis()) {
        size_t axisCount = m_definition.xAxis().count();
        m_xAxis.resize(axisCount);
        size_t axisElementSize = (m_definition.xAxis().dataType() == 1 ? 1 : 
                                  (m_definition.xAxis().dataType() == 4 ? 4 : 2));
        
        for (size_t i = 0; i < axisCount && offset + axisElementSize <= dataSize; ++i) {
            if (axisElementSize == 2) {
                uint16_t rawValue = EndiannessConverter::readLittleEndian<uint16_t>(data + offset);
                m_xAxis[i] = ScalingEngine::rawToPhysical(rawValue, m_definition.xAxis().factor(), m_definition.xAxis().offset());
            } else if (axisElementSize == 1) {
                uint8_t rawValue = data[offset];
                m_xAxis[i] = ScalingEngine::rawToPhysical(rawValue, m_definition.xAxis().factor(), m_definition.xAxis().offset());
            } else if (axisElementSize == 4) {
                float fval = EndiannessConverter::readLittleEndian<float>(data + offset);
                m_xAxis[i] = ScalingEngine::rawToPhysical(fval, m_definition.xAxis().factor(), m_definition.xAxis().offset());
            }
            offset += axisElementSize;
        }
    }
    
    // Load Y axis if present
    if (m_definition.hasYAxis()) {
        size_t axisCount = m_definition.yAxis().count();
        m_yAxis.resize(axisCount);
        size_t axisElementSize = (m_definition.yAxis().dataType() == 1 ? 1 : 
                                  (m_definition.yAxis().dataType() == 4 ? 4 : 2));
        
        for (size_t i = 0; i < axisCount && offset + axisElementSize <= dataSize; ++i) {
            if (axisElementSize == 2) {
                uint16_t rawValue = EndiannessConverter::readLittleEndian<uint16_t>(data + offset);
                m_yAxis[i] = ScalingEngine::rawToPhysical(rawValue, m_definition.yAxis().factor(), m_definition.yAxis().offset());
            } else if (axisElementSize == 1) {
                uint8_t rawValue = data[offset];
                m_yAxis[i] = ScalingEngine::rawToPhysical(rawValue, m_definition.yAxis().factor(), m_definition.yAxis().offset());
            } else if (axisElementSize == 4) {
                float fval = EndiannessConverter::readLittleEndian<float>(data + offset);
                m_yAxis[i] = ScalingEngine::rawToPhysical(fval, m_definition.yAxis().factor(), m_definition.yAxis().offset());
            }
            offset += axisElementSize;
        }
    }
    
    // Load data
    size_t elementSize = m_definition.dataSize();
    size_t dataCount = m_rows * m_columns;
    
    for (size_t i = 0; i < dataCount && offset + elementSize <= dataSize; ++i) {
        if (elementSize == 2) {
            m_data[i] = EndiannessConverter::readLittleEndian<uint16_t>(data + offset);
        } else if (elementSize == 1) {
            m_data[i] = data[offset];
        } else if (elementSize == 4) {
            float fval = EndiannessConverter::readLittleEndian<float>(data + offset);
            m_data[i] = static_cast<uint16_t>(ScalingEngine::rawToPhysical(fval, m_definition.factor(), m_definition.offset()) * 1000.0);
        }
        offset += elementSize;
    }
}

void Map3D::writeToBinary(uint8_t* data, size_t dataSize) const {
    if (!data || dataSize == 0) {
        return;
    }
    
    size_t offset = 0;
    
    // Write X axis if present
    if (m_definition.hasXAxis()) {
        size_t axisCount = m_xAxis.size();
        size_t axisElementSize = (m_definition.xAxis().dataType() == 1 ? 1 : 
                                  (m_definition.xAxis().dataType() == 4 ? 4 : 2));
        
        for (size_t i = 0; i < axisCount && offset + axisElementSize <= dataSize; ++i) {
            if (axisElementSize == 2) {
                uint16_t rawValue = ScalingEngine::physicalToRaw(m_xAxis[i], m_definition.xAxis().factor(), m_definition.xAxis().offset());
                EndiannessConverter::writeLittleEndian<uint16_t>(data + offset, rawValue);
            } else if (axisElementSize == 1) {
                uint8_t rawValue = ScalingEngine::physicalToRaw(m_xAxis[i], m_definition.xAxis().factor(), m_definition.xAxis().offset());
                data[offset] = rawValue;
            } else if (axisElementSize == 4) {
                float rawValue = ScalingEngine::physicalToRaw(m_xAxis[i], m_definition.xAxis().factor(), m_definition.xAxis().offset());
                EndiannessConverter::writeLittleEndian<float>(data + offset, rawValue);
            }
            offset += axisElementSize;
        }
    }
    
    // Write Y axis if present
    if (m_definition.hasYAxis()) {
        size_t axisCount = m_yAxis.size();
        size_t axisElementSize = (m_definition.yAxis().dataType() == 1 ? 1 : 
                                  (m_definition.yAxis().dataType() == 4 ? 4 : 2));
        
        for (size_t i = 0; i < axisCount && offset + axisElementSize <= dataSize; ++i) {
            if (axisElementSize == 2) {
                uint16_t rawValue = ScalingEngine::physicalToRaw(m_yAxis[i], m_definition.yAxis().factor(), m_definition.yAxis().offset());
                EndiannessConverter::writeLittleEndian<uint16_t>(data + offset, rawValue);
            } else if (axisElementSize == 1) {
                uint8_t rawValue = ScalingEngine::physicalToRaw(m_yAxis[i], m_definition.yAxis().factor(), m_definition.yAxis().offset());
                data[offset] = rawValue;
            } else if (axisElementSize == 4) {
                float rawValue = ScalingEngine::physicalToRaw(m_yAxis[i], m_definition.yAxis().factor(), m_definition.yAxis().offset());
                EndiannessConverter::writeLittleEndian<float>(data + offset, rawValue);
            }
            offset += axisElementSize;
        }
    }
    
    // Write data
    size_t elementSize = m_definition.dataSize();
    size_t dataCount = m_data.size();
    
    for (size_t i = 0; i < dataCount && offset + elementSize <= dataSize; ++i) {
        if (elementSize == 2) {
            EndiannessConverter::writeLittleEndian<uint16_t>(data + offset, m_data[i]);
        } else if (elementSize == 1) {
            data[offset] = static_cast<uint8_t>(m_data[i]);
        } else if (elementSize == 4) {
            size_t row = i / m_columns;
            size_t col = i % m_columns;
            double physical = getPhysicalValue(row, col);
            float rawValue = ScalingEngine::physicalToRaw(physical, m_definition.factor(), m_definition.offset());
            EndiannessConverter::writeLittleEndian<float>(data + offset, rawValue);
        }
        offset += elementSize;
    }
}

uint16_t Map3D::getRawValue(size_t row, size_t col) const {
    if (row >= m_rows || col >= m_columns) {
        return 0;
    }
    return m_data[indexOf(row, col)];
}

void Map3D::setRawValue(size_t row, size_t col, uint16_t value) {
    if (row >= m_rows || col >= m_columns) {
        return;
    }
    m_data[indexOf(row, col)] = value;
}

double Map3D::getPhysicalValue(size_t row, size_t col) const {
    if (row >= m_rows || col >= m_columns) {
        return 0.0;
    }
    return ScalingEngine::rawToPhysical(m_data[indexOf(row, col)], m_definition.factor(), m_definition.offset());
}

void Map3D::setPhysicalValue(size_t row, size_t col, double value) {
    if (row >= m_rows || col >= m_columns) {
        return;
    }
    m_data[indexOf(row, col)] = ScalingEngine::physicalToRaw(value, m_definition.factor(), m_definition.offset());
}

double Map3D::getXAxisValue(size_t index) const {
    if (index >= m_xAxis.size()) {
        return static_cast<double>(index);
    }
    return m_xAxis[index];
}

void Map3D::setXAxisValue(size_t index, double value) {
    if (index >= m_xAxis.size()) {
        return;
    }
    m_xAxis[index] = value;
}

double Map3D::getYAxisValue(size_t index) const {
    if (index >= m_yAxis.size()) {
        return static_cast<double>(index);
    }
    return m_yAxis[index];
}

void Map3D::setYAxisValue(size_t index, double value) {
    if (index >= m_yAxis.size()) {
        return;
    }
    m_yAxis[index] = value;
}

} // namespace WinMMM10

