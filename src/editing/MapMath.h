#pragma once

#include "../maps/MapDefinition.h"
#include "../binary/BinaryFile.h"
#include <functional>

namespace WinMMM10 {

class MapMath {
public:
    MapMath(BinaryFile* file);
    ~MapMath() = default;
    
    enum Operation {
        Add,
        Subtract,
        Multiply,
        Divide
    };
    
    // Map-to-map operations
    bool addMaps(const MapDefinition& map1, const MapDefinition& map2, const MapDefinition& resultMap);
    bool subtractMaps(const MapDefinition& map1, const MapDefinition& map2, const MapDefinition& resultMap);
    bool multiplyMaps(const MapDefinition& map1, const MapDefinition& map2, const MapDefinition& resultMap);
    bool divideMaps(const MapDefinition& map1, const MapDefinition& map2, const MapDefinition& resultMap);
    
    // Map-to-scalar operations
    bool addScalar(const MapDefinition& map, double scalar);
    bool subtractScalar(const MapDefinition& map, double scalar);
    bool multiplyScalar(const MapDefinition& map, double scalar);
    bool divideScalar(const MapDefinition& map, double scalar);
    
    // Apply function to map
    bool applyFunction(const MapDefinition& map, std::function<double(double)> func);

private:
    double readMapValue(const MapDefinition& map, size_t row, size_t col) const;
    void writeMapValue(const MapDefinition& map, size_t row, size_t col, double value);
    
    BinaryFile* m_binaryFile;
};

} // namespace WinMMM10

