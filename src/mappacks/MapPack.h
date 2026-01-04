#pragma once

#include "../maps/MapDefinition.h"
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace WinMMM10 {

struct MapPackInfo {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string ecuName;
    std::string ecuId;
    std::string fileHash; // MD5/SHA256 of original binary
    std::vector<std::string> tags;
};

class MapPack {
public:
    MapPack();
    ~MapPack() = default;
    
    const MapPackInfo& info() const { return m_info; }
    MapPackInfo& info() { return m_info; }
    
    void addMap(const MapDefinition& map);
    void removeMap(size_t index);
    MapDefinition& getMap(size_t index);
    const MapDefinition& getMap(size_t index) const;
    size_t mapCount() const { return m_maps.size(); }
    
    std::vector<MapDefinition>& maps() { return m_maps; }
    const std::vector<MapDefinition>& maps() const { return m_maps; }
    
    bool saveToFile(const std::string& filepath) const;
    bool loadFromFile(const std::string& filepath);
    
    static std::string getFileExtension() { return ".mappack"; }

private:
    MapPackInfo m_info;
    std::vector<MapDefinition> m_maps;
};

class MapPackManager {
public:
    static MapPackManager& instance();
    
    bool loadMapPack(const std::string& filepath);
    bool saveMapPack(const MapPack& pack, const std::string& filepath);
    std::vector<MapPackInfo> getInstalledMapPacks() const;
    
private:
    MapPackManager() = default;
    ~MapPackManager() = default;
    MapPackManager(const MapPackManager&) = delete;
    MapPackManager& operator=(const MapPackManager&) = delete;
    
    std::vector<MapPack> m_installedPacks;
};

} // namespace WinMMM10

