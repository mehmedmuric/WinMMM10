#pragma once

#include "../maps/MapDefinition.h"
#include "../binary/BinaryFile.h"
#include <vector>
#include <functional>
#include <cstdint>
#include <cstddef>

namespace WinMMM10 {

class BatchOperations {
public:
    BatchOperations(BinaryFile* file);
    ~BatchOperations() = default;
    
    enum FillMode {
        Constant,
        Linear,
        Interpolation
    };
    
    // Copy/Paste operations
    bool copyMapData(const MapDefinition& map, size_t startRow, size_t startCol,
                    size_t endRow, size_t endCol, std::vector<double>& buffer);
    bool pasteMapData(const MapDefinition& map, size_t startRow, size_t startCol,
                     const std::vector<double>& buffer);
    
    // Fill operations
    bool fillMap(const MapDefinition& map, double value, FillMode mode = Constant);
    bool fillMapRegion(const MapDefinition& map, size_t startRow, size_t startCol,
                      size_t endRow, size_t endCol, double value, FillMode mode = Constant);
    
    // Batch operations
    size_t applyToAllMaps(const std::vector<MapDefinition>& maps,
                         std::function<void(const MapDefinition&, BinaryFile*)> operation);

private:
    double readMapValue(const MapDefinition& map, size_t row, size_t col) const;
    void writeMapValue(const MapDefinition& map, size_t row, size_t col, double value);
    
    BinaryFile* m_binaryFile;
};

} // namespace WinMMM10

