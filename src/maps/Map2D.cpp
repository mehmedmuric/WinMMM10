#include "Map2D.h"
#include "ScalingEngine.h"
#include "binary/Endianness.h"
#include <cstring>
#include <algorithm>

namespace WinMMM10 {

Map2D::Map2D() = default;

Map2D::Map2D(const MapDefinition& definition) : m_definition(definition) {
    m_data.resize(definition.columns());
    if (definition.hasXAxis()) {
        m_xAxis.resize(definition.xAxis().count());
    }
}

void Map2D::loadFromBinary(const uint8_t* data, size_t dataSize) {
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
            uint16_t rawValue = 0;
            if (axisElementSize == 2) {
                rawValue = EndiannessConverter::readLittleEndian<uint16_t>(data + offset);
            } else if (axisElementSize == 1) {
                rawValue = data[offset];
            } else if (axisElementSize == 4) {
                float fval = EndiannessConverter::readLittleEndian<float>(data + offset);
                m_xAxis[i] = ScalingEngine::rawToPhysical(fval, m_definition.xAxis().factor(), m_definition.xAxis().offset());
                offset += axisElementSize;
                continue;
            }
            m_xAxis[i] = ScalingEngine::rawToPhysical(rawValue, m_definition.xAxis().factor(), m_definition.xAxis().offset());
            offset += axisElementSize;
        }
    }
    
    // Load data
    size_t pointCount = m_definition.columns();
    m_data.resize(pointCount);
    size_t elementSize = m_definition.dataSize();
    
    for (size_t i = 0; i < pointCount && offset + elementSize <= dataSize; ++i) {
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

void Map2D::writeToBinary(uint8_t* data, size_t dataSize) const {
    if (!data || dataSize == 0) {
        return;
    }
    
    size_t offset = 0;
    
    // Write X axis if present
    if (m_definition.hasXAxis()) {
        size_t axisCount = m_definition.xAxis().count();
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
    
    // Write data
    size_t pointCount = m_data.size();
    size_t elementSize = m_definition.dataSize();
    
    for (size_t i = 0; i < pointCount && offset + elementSize <= dataSize; ++i) {
        if (elementSize == 2) {
            EndiannessConverter::writeLittleEndian<uint16_t>(data + offset, m_data[i]);
        } else if (elementSize == 1) {
            data[offset] = static_cast<uint8_t>(m_data[i]);
        } else if (elementSize == 4) {
            double physical = getPhysicalValue(i);
            float rawValue = ScalingEngine::physicalToRaw(physical, m_definition.factor(), m_definition.offset());
            EndiannessConverter::writeLittleEndian<float>(data + offset, rawValue);
        }
        offset += elementSize;
    }
}

uint16_t Map2D::getRawValue(size_t index) const {
    if (index >= m_data.size()) {
        return 0;
    }
    return m_data[index];
}

void Map2D::setRawValue(size_t index, uint16_t value) {
    if (index >= m_data.size()) {
        return;
    }
    m_data[index] = value;
}

double Map2D::getPhysicalValue(size_t index) const {
    if (index >= m_data.size()) {
        return 0.0;
    }
    return ScalingEngine::rawToPhysical(m_data[index], m_definition.factor(), m_definition.offset());
}

void Map2D::setPhysicalValue(size_t index, double value) {
    if (index >= m_data.size()) {
        return;
    }
    m_data[index] = ScalingEngine::physicalToRaw(value, m_definition.factor(), m_definition.offset());
}

double Map2D::getXAxisValue(size_t index) const {
    if (index >= m_xAxis.size()) {
        return static_cast<double>(index);
    }
    return m_xAxis[index];
}

void Map2D::setXAxisValue(size_t index, double value) {
    if (index >= m_xAxis.size()) {
        return;
    }
    m_xAxis[index] = value;
}

} // namespace WinMMM10

