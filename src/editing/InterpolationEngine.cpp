#include "InterpolationEngine.h"
#include <algorithm>
#include <cmath>
#include <vector>

namespace WinMMM10 {

InterpolationEngine::InterpolationEngine(BinaryFile* file)
    : m_binaryFile(file)
{
}

double InterpolationEngine::readMapValue(const MapDefinition& map, size_t row, size_t col) const {
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

void InterpolationEngine::writeMapValue(const MapDefinition& map, size_t row, size_t col, double value) {
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

double InterpolationEngine::linearInterpolate(double x0, double y0, double x1, double y1, double x) const {
    if (std::abs(x1 - x0) < 0.0001) {
        return y0;
    }
    double t = (x - x0) / (x1 - x0);
    return y0 + (y1 - y0) * t;
}

double InterpolationEngine::cubicInterpolate(double p0, double p1, double p2, double p3, double t) const {
    double t2 = t * t;
    double t3 = t2 * t;
    
    return 0.5 * (2.0 * p1 +
                  (-p0 + p2) * t +
                  (2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3) * t2 +
                  (-p0 + 3.0 * p1 - 3.0 * p2 + p3) * t3);
}

bool InterpolationEngine::interpolateMap(const MapDefinition& map, InterpolationType type) {
    return interpolateRegion(map, 0, 0, map.rows() - 1, map.columns() - 1, type);
}

bool InterpolationEngine::interpolateRegion(const MapDefinition& map, size_t startRow, size_t startCol,
                                           size_t endRow, size_t endCol, InterpolationType type) {
    if (type == InterpolationType::Linear) {
        // Simple linear interpolation between corner points
        double topLeft = readMapValue(map, startRow, startCol);
        double topRight = readMapValue(map, startRow, endCol);
        double bottomLeft = readMapValue(map, endRow, startCol);
        double bottomRight = readMapValue(map, endRow, endCol);
        
        for (size_t r = startRow; r <= endRow; ++r) {
            for (size_t c = startCol; c <= endCol; ++c) {
                double rowT = static_cast<double>(r - startRow) / (endRow - startRow);
                double colT = static_cast<double>(c - startCol) / (endCol - startCol);
                
                double top = linearInterpolate(0, topLeft, 1, topRight, colT);
                double bottom = linearInterpolate(0, bottomLeft, 1, bottomRight, colT);
                double value = linearInterpolate(0, top, 1, bottom, rowT);
                
                writeMapValue(map, r, c, value);
            }
        }
    }
    
    return true;
}

bool InterpolationEngine::smoothMap(const MapDefinition& map, int kernelSize) {
    if (kernelSize < 3 || kernelSize % 2 == 0) {
        return false;
    }
    
    size_t rows = map.rows();
    size_t cols = map.columns();
    std::vector<std::vector<double>> temp(rows, std::vector<double>(cols));
    
    int halfKernel = kernelSize / 2;
    
    // Read all values
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            temp[r][c] = readMapValue(map, r, c);
        }
    }
    
    // Apply smoothing (simple box filter)
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            double sum = 0.0;
            int count = 0;
            
            for (int dr = -halfKernel; dr <= halfKernel; ++dr) {
                for (int dc = -halfKernel; dc <= halfKernel; ++dc) {
                    int nr = static_cast<int>(r) + dr;
                    int nc = static_cast<int>(c) + dc;
                    
                    if (nr >= 0 && nr < static_cast<int>(rows) &&
                        nc >= 0 && nc < static_cast<int>(cols)) {
                        sum += temp[nr][nc];
                        count++;
                    }
                }
            }
            
            if (count > 0) {
                writeMapValue(map, r, c, sum / count);
            }
        }
    }
    
    return true;
}

} // namespace WinMMM10

