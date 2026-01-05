#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <map>

namespace WinMMM10 {

class MapDefinition;

struct CachedMapData {
    std::string mapName;
    size_t address;
    std::vector<uint8_t> rawData;
    std::vector<double> processedData;
    int64_t timestamp{0};
};

struct DetectionCache {
    std::string binaryHash;
    std::vector<CachedMapData> detectedMaps;
    int64_t timestamp{0};
};

class ProjectCache {
public:
    ProjectCache(const std::string& projectPath);
    ~ProjectCache() = default;
    
    // Map detection cache
    void cacheDetectionResults(const std::string& binaryHash, 
                               const std::vector<CachedMapData>& maps);
    bool hasDetectionCache(const std::string& binaryHash) const;
    std::vector<CachedMapData> getDetectionCache(const std::string& binaryHash) const;
    void clearDetectionCache();
    
    // Compiled map cache
    void cacheMapData(const std::string& mapName, const CachedMapData& data);
    bool hasMapCache(const std::string& mapName) const;
    CachedMapData getMapCache(const std::string& mapName) const;
    void clearMapCache();
    
    // Cache size
    uint64_t getCacheSize() const;
    void clearAll();
    
    // Load/Save
    void load();
    void save();

private:
    std::string getCacheFilePath() const;
    uint64_t calculateCacheSize() const;
    
    std::string m_projectPath;
    std::map<std::string, DetectionCache> m_detectionCache;
    std::map<std::string, CachedMapData> m_mapCache;
};

} // namespace WinMMM10

