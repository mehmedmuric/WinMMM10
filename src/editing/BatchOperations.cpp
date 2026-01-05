#include "BatchOperations.h"
#include <algorithm>
#include <functional>

namespace WinMMM10 {

BatchOperations::BatchOperations(BinaryFile* file)
    : m_binaryFile(file)
{
}

double BatchOperations::readMapValue(const MapDefinition& map, size_t row, size_t col) const {
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return 0.0;
    }
    
    size_t address = map.address();
    size_t index = row * map.columns() + col;
    
    switch (map.dataType()) {
        case 1: // uint8
            address += index;
            if (address < m_binaryFile->size()) {
                uint8_t value = m_binaryFile->readUInt8(address);
                return static_cast<double>(value) * map.factor() + map.offset();
            }
            break;
        case 2: // uint16
            address += index * 2;
            if (address + 1 < m_binaryFile->size()) {
                uint16_t value = m_binaryFile->readUInt16(address);
                return static_cast<double>(value) * map.factor() + map.offset();
            }
            break;
        case 3: // int16
            address += index * 2;
            if (address + 1 < m_binaryFile->size()) {
                int16_t value = static_cast<int16_t>(m_binaryFile->readUInt16(address));
                return static_cast<double>(value) * map.factor() + map.offset();
            }
            break;
        case 4: // float
            address += index * 4;
            if (address + 3 < m_binaryFile->size()) {
                return m_binaryFile->readFloat(address) * map.factor() + map.offset();
            }
            break;
    }
    
    return 0.0;
}

void BatchOperations::writeMapValue(const MapDefinition& map, size_t row, size_t col, double value) {
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return;
    }
    
    // Convert from physical value to raw value
    double rawValue = (value - map.offset()) / map.factor();
    
    size_t address = map.address();
    size_t index = row * map.columns() + col;
    
    switch (map.dataType()) {
        case 1: // uint8
            address += index;
            if (address < m_binaryFile->size()) {
                m_binaryFile->writeUInt8(address, static_cast<uint8_t>(std::round(rawValue)));
            }
            break;
        case 2: // uint16
            address += index * 2;
            if (address + 1 < m_binaryFile->size()) {
                m_binaryFile->writeUInt16(address, static_cast<uint16_t>(std::round(rawValue)));
            }
            break;
        case 3: // int16
            address += index * 2;
            if (address + 1 < m_binaryFile->size()) {
                m_binaryFile->writeInt16(address, static_cast<int16_t>(std::round(rawValue)));
            }
            break;
        case 4: // float
            address += index * 4;
            if (address + 3 < m_binaryFile->size()) {
                m_binaryFile->writeFloat(address, static_cast<float>(rawValue));
            }
            break;
    }
}

bool BatchOperations::copyMapData(const MapDefinition& map, size_t startRow, size_t startCol,
                                  size_t endRow, size_t endCol, std::vector<double>& buffer) {
    buffer.clear();
    
    size_t rows = map.rows();
    size_t cols = map.columns();
    
    if (startRow >= rows || startCol >= cols || endRow >= rows || endCol >= cols) {
        return false;
    }
    
    if (startRow > endRow || startCol > endCol) {
        return false;
    }
    
    size_t regionRows = endRow - startRow + 1;
    size_t regionCols = endCol - startCol + 1;
    buffer.reserve(regionRows * regionCols);
    
    for (size_t r = startRow; r <= endRow; ++r) {
        for (size_t c = startCol; c <= endCol; ++c) {
            buffer.push_back(readMapValue(map, r, c));
        }
    }
    
    return true;
}

bool BatchOperations::pasteMapData(const MapDefinition& map, size_t startRow, size_t startCol,
                                  const std::vector<double>& buffer) {
    size_t rows = map.rows();
    size_t cols = map.columns();
    
    if (startRow >= rows || startCol >= cols) {
        return false;
    }
    
    size_t bufferIndex = 0;
    for (size_t r = startRow; r < rows && bufferIndex < buffer.size(); ++r) {
        for (size_t c = startCol; c < cols && bufferIndex < buffer.size(); ++c) {
            writeMapValue(map, r, c, buffer[bufferIndex]);
            bufferIndex++;
        }
    }
    
    return true;
}

bool BatchOperations::fillMap(const MapDefinition& map, double value, FillMode mode) {
    return fillMapRegion(map, 0, 0, map.rows() - 1, map.columns() - 1, value, mode);
}

bool BatchOperations::fillMapRegion(const MapDefinition& map, size_t startRow, size_t startCol,
                                   size_t endRow, size_t endCol, double value, FillMode mode) {
    size_t rows = map.rows();
    size_t cols = map.columns();
    
    if (startRow >= rows || startCol >= cols || endRow >= rows || endCol >= cols) {
        return false;
    }
    
    if (mode == Constant) {
        for (size_t r = startRow; r <= endRow; ++r) {
            for (size_t c = startCol; c <= endCol; ++c) {
                writeMapValue(map, r, c, value);
            }
        }
    } else if (mode == Linear) {
        // Simple linear interpolation from start to end
        double startValue = readMapValue(map, startRow, startCol);
        double endValue = value;
        size_t totalCells = (endRow - startRow + 1) * (endCol - startCol + 1);
        size_t cellIndex = 0;
        
        for (size_t r = startRow; r <= endRow; ++r) {
            for (size_t c = startCol; c <= endCol; ++c) {
                double t = static_cast<double>(cellIndex) / (totalCells - 1);
                double interpolated = startValue + (endValue - startValue) * t;
                writeMapValue(map, r, c, interpolated);
                cellIndex++;
            }
        }
    }
    
    return true;
}

size_t BatchOperations::applyToAllMaps(const std::vector<MapDefinition>& maps,
                                      std::function<void(const MapDefinition&, BinaryFile*)> operation) {
    size_t count = 0;
    for (const auto& map : maps) {
        operation(map, m_binaryFile);
        count++;
    }
    return count;
}

} // namespace WinMMM10

