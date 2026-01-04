#pragma once

#include "../maps/MapDefinition.h"
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace WinMMM10 {

class Project {
public:
    Project();
    ~Project() = default;
    
    std::string name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    
    std::string filepath() const { return m_filepath; }
    void setFilepath(const std::string& filepath) { m_filepath = filepath; }
    
    std::string binaryFilepath() const { return m_binaryFilepath; }
    void setBinaryFilepath(const std::string& filepath) { m_binaryFilepath = filepath; }
    
    std::string ecuName() const { return m_ecuName; }
    void setEcuName(const std::string& name) { m_ecuName = name; }
    
    std::string description() const { return m_description; }
    void setDescription(const std::string& desc) { m_description = desc; }
    
    void addMap(const MapDefinition& map);
    void removeMap(size_t index);
    MapDefinition& getMap(size_t index);
    const MapDefinition& getMap(size_t index) const;
    size_t mapCount() const { return m_maps.size(); }
    
    std::vector<MapDefinition>& maps() { return m_maps; }
    const std::vector<MapDefinition>& maps() const { return m_maps; }

private:
    std::string m_name;
    std::string m_filepath;
    std::string m_binaryFilepath;
    std::string m_ecuName;
    std::string m_description;
    std::vector<MapDefinition> m_maps;
};

} // namespace WinMMM10

