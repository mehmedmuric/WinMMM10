#include "MapComparator.h"
#include "../maps/Map2D.h"
#include "../maps/Map3D.h"
#include <algorithm>
#include <cmath>

namespace WinMMM10 {

double MapComparator::readMapValue(const MapDefinition& map, const BinaryFile* file,
                                   size_t row, size_t col) const {
    if (!file || !file->isLoaded()) {
        return 0.0;
    }
    
    size_t address = map.address();
    size_t dataSize = 0;
    
    switch (map.dataType()) {
        case 1: // uint8
            dataSize = 1;
            break;
        case 2: // uint16
            dataSize = 2;
            address += (row * map.columns() + col) * 2;
            if (address + 1 < file->size()) {
                uint16_t value = file->readUInt16(address);
                return static_cast<double>(value) * map.factor() + map.offset();
            }
            break;
        case 3: // int16
            dataSize = 2;
            address += (row * map.columns() + col) * 2;
            if (address + 1 < file->size()) {
                int16_t value = static_cast<int16_t>(file->readUInt16(address));
                return static_cast<double>(value) * map.factor() + map.offset();
            }
            break;
        case 4: // float
            dataSize = 4;
            address += (row * map.columns() + col) * 4;
            if (address + 3 < file->size()) {
                return file->readFloat(address) * map.factor() + map.offset();
            }
            break;
    }
    
    return 0.0;
}

double MapComparator::calculateDifference(double val1, double val2) const {
    return std::abs(val1 - val2);
}

MapComparator::ComparisonResult MapComparator::compareMaps(const MapDefinition& map1,
                                                          const BinaryFile* file1,
                                                          const MapDefinition& map2,
                                                          const BinaryFile* file2) {
    ComparisonResult result;
    
    // Check if maps have same dimensions
    if (map1.rows() != map2.rows() || map1.columns() != map2.columns()) {
        return result; // Maps are incompatible
    }
    
    size_t rows = map1.rows();
    size_t cols = map1.columns();
    double totalDiff = 0.0;
    size_t diffCount = 0;
    
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            double val1 = readMapValue(map1, file1, r, c);
            double val2 = readMapValue(map2, file2, r, c);
            double diff = calculateDifference(val1, val2);
            
            MapDifference mapDiff;
            mapDiff.row = r;
            mapDiff.column = c;
            mapDiff.originalValue = val1;
            mapDiff.modifiedValue = val2;
            mapDiff.difference = diff;
            mapDiff.isDifferent = (diff > 0.0001); // Tolerance
            
            if (mapDiff.isDifferent) {
                result.differences.push_back(mapDiff);
                totalDiff += diff;
                diffCount++;
                
                if (diff > result.maxDifference) {
                    result.maxDifference = diff;
                }
                if (result.minDifference == 0.0 || diff < result.minDifference) {
                    result.minDifference = diff;
                }
            }
        }
    }
    
    result.totalDifferences = diffCount;
    if (diffCount > 0) {
        result.averageDifference = totalDiff / diffCount;
    }
    
    return result;
}

bool MapComparator::mapsAreEqual(const MapDefinition& map1, const BinaryFile* file1,
                                 const MapDefinition& map2, const BinaryFile* file2,
                                 double tolerance) {
    if (map1.rows() != map2.rows() || map1.columns() != map2.columns()) {
        return false;
    }
    
    size_t rows = map1.rows();
    size_t cols = map1.columns();
    
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            double val1 = readMapValue(map1, file1, r, c);
            double val2 = readMapValue(map2, file2, r, c);
            double diff = calculateDifference(val1, val2);
            
            if (diff > tolerance) {
                return false;
            }
        }
    }
    
    return true;
}

} // namespace WinMMM10

