#pragma once

#include "../maps/MapDefinition.h"
#include "../binary/BinaryFile.h"
#include <vector>
#include <cstdint>
#include <cstddef>

namespace WinMMM10 {

struct MapDifference {
    size_t row{0};
    size_t column{0};
    double originalValue{0.0};
    double modifiedValue{0.0};
    double difference{0.0};
    bool isDifferent{false};
};

class MapComparator {
public:
    MapComparator() = default;
    ~MapComparator() = default;
    
    struct ComparisonResult {
        std::vector<MapDifference> differences;
        size_t totalDifferences{0};
        double maxDifference{0.0};
        double minDifference{0.0};
        double averageDifference{0.0};
    };
    
    ComparisonResult compareMaps(const MapDefinition& map1, const BinaryFile* file1,
                                 const MapDefinition& map2, const BinaryFile* file2);
    
    bool mapsAreEqual(const MapDefinition& map1, const BinaryFile* file1,
                     const MapDefinition& map2, const BinaryFile* file2,
                     double tolerance = 0.0001);

private:
    double readMapValue(const MapDefinition& map, const BinaryFile* file,
                       size_t row, size_t col) const;
    double calculateDifference(double val1, double val2) const;
};

} // namespace WinMMM10

