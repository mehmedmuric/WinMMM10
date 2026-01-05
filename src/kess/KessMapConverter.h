#pragma once

#include "KessFileReader.h"
#include "KessFileWriter.h"
#include "../maps/MapDefinition.h"
#include "../core/Project.h"
#include <string>

namespace WinMMM10 {

class KessMapConverter {
public:
    KessMapConverter();
    ~KessMapConverter() = default;
    
    // Convert KESS file to project
    bool importKessFile(const std::string& kessFilepath, Project& project);
    
    // Convert project to KESS file
    bool exportToKessFile(const Project& project, const std::string& kessFilepath);
    
    // Convert map definitions
    std::vector<MapDefinition> convertKessMaps(const std::vector<MapDefinition>& kessMaps);
    std::vector<MapDefinition> convertToKessMaps(const std::vector<MapDefinition>& maps);

private:
    MapDefinition convertMap(const MapDefinition& map);
};

} // namespace WinMMM10

