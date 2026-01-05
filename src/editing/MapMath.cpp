#include "MapMath.h"
#include <algorithm>
#include <cmath>

namespace WinMMM10 {

MapMath::MapMath(BinaryFile* file)
    : m_binaryFile(file)
{
}

double MapMath::readMapValue(const MapDefinition& map, size_t row, size_t col) const {
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

void MapMath::writeMapValue(const MapDefinition& map, size_t row, size_t col, double value) {
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return;
    }
    
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

bool MapMath::addMaps(const MapDefinition& map1, const MapDefinition& map2, const MapDefinition& resultMap) {
    if (map1.rows() != map2.rows() || map1.columns() != map2.columns() ||
        map1.rows() != resultMap.rows() || map1.columns() != resultMap.columns()) {
        return false;
    }
    
    for (size_t r = 0; r < map1.rows(); ++r) {
        for (size_t c = 0; c < map1.columns(); ++c) {
            double val1 = readMapValue(map1, r, c);
            double val2 = readMapValue(map2, r, c);
            writeMapValue(resultMap, r, c, val1 + val2);
        }
    }
    
    return true;
}

bool MapMath::subtractMaps(const MapDefinition& map1, const MapDefinition& map2, const MapDefinition& resultMap) {
    if (map1.rows() != map2.rows() || map1.columns() != map2.columns() ||
        map1.rows() != resultMap.rows() || map1.columns() != resultMap.columns()) {
        return false;
    }
    
    for (size_t r = 0; r < map1.rows(); ++r) {
        for (size_t c = 0; c < map1.columns(); ++c) {
            double val1 = readMapValue(map1, r, c);
            double val2 = readMapValue(map2, r, c);
            writeMapValue(resultMap, r, c, val1 - val2);
        }
    }
    
    return true;
}

bool MapMath::multiplyMaps(const MapDefinition& map1, const MapDefinition& map2, const MapDefinition& resultMap) {
    if (map1.rows() != map2.rows() || map1.columns() != map2.columns() ||
        map1.rows() != resultMap.rows() || map1.columns() != resultMap.columns()) {
        return false;
    }
    
    for (size_t r = 0; r < map1.rows(); ++r) {
        for (size_t c = 0; c < map1.columns(); ++c) {
            double val1 = readMapValue(map1, r, c);
            double val2 = readMapValue(map2, r, c);
            writeMapValue(resultMap, r, c, val1 * val2);
        }
    }
    
    return true;
}

bool MapMath::divideMaps(const MapDefinition& map1, const MapDefinition& map2, const MapDefinition& resultMap) {
    if (map1.rows() != map2.rows() || map1.columns() != map2.columns() ||
        map1.rows() != resultMap.rows() || map1.columns() != resultMap.columns()) {
        return false;
    }
    
    for (size_t r = 0; r < map1.rows(); ++r) {
        for (size_t c = 0; c < map1.columns(); ++c) {
            double val1 = readMapValue(map1, r, c);
            double val2 = readMapValue(map2, r, c);
            if (std::abs(val2) > 0.0001) {
                writeMapValue(resultMap, r, c, val1 / val2);
            }
        }
    }
    
    return true;
}

bool MapMath::addScalar(const MapDefinition& map, double scalar) {
    for (size_t r = 0; r < map.rows(); ++r) {
        for (size_t c = 0; c < map.columns(); ++c) {
            double val = readMapValue(map, r, c);
            writeMapValue(map, r, c, val + scalar);
        }
    }
    return true;
}

bool MapMath::subtractScalar(const MapDefinition& map, double scalar) {
    return addScalar(map, -scalar);
}

bool MapMath::multiplyScalar(const MapDefinition& map, double scalar) {
    for (size_t r = 0; r < map.rows(); ++r) {
        for (size_t c = 0; c < map.columns(); ++c) {
            double val = readMapValue(map, r, c);
            writeMapValue(map, r, c, val * scalar);
        }
    }
    return true;
}

bool MapMath::divideScalar(const MapDefinition& map, double scalar) {
    if (std::abs(scalar) < 0.0001) {
        return false;
    }
    return multiplyScalar(map, 1.0 / scalar);
}

bool MapMath::applyFunction(const MapDefinition& map, std::function<double(double)> func) {
    for (size_t r = 0; r < map.rows(); ++r) {
        for (size_t c = 0; c < map.columns(); ++c) {
            double val = readMapValue(map, r, c);
            writeMapValue(map, r, c, func(val));
        }
    }
    return true;
}

} // namespace WinMMM10

