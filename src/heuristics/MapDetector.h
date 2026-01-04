#pragma once

#include "../maps/MapDefinition.h"
#include <vector>
#include <cstdint>
#include <cstddef>

namespace WinMMM10 {

struct MapCandidate {
    size_t address{0};
    MapType type{MapType::Map2D};
    size_t rows{0};
    size_t columns{0};
    double confidence{0.0};
};

class MapDetector {
public:
    MapDetector();
    ~MapDetector() = default;
    
    void setBinaryData(const uint8_t* data, size_t size);
    std::vector<MapCandidate> detectMaps(size_t minAddress = 0, size_t maxAddress = 0);
    
private:
    MapCandidate detect2DMap(size_t offset);
    MapCandidate detect3DMap(size_t offset);
    bool isValidMapAddress(size_t offset, const MapCandidate& candidate);
    
    const uint8_t* m_data{nullptr};
    size_t m_size{0};
};

} // namespace WinMMM10

