#include "KessMapConverter.h"

namespace WinMMM10 {

KessMapConverter::KessMapConverter() {
}

bool KessMapConverter::importKessFile(const std::string& kessFilepath, Project& project) {
    KessFileReader reader;
    if (!reader.readFile(kessFilepath)) {
        return false;
    }
    
    // Set ECU info
    const auto& ecuInfo = reader.getEcuInfo();
    project.setEcuName(ecuInfo.ecuName);
    project.setDescription("Imported from KESS file: " + ecuInfo.ecuType);
    
    // Convert and add maps
    auto kessMaps = reader.getMapDefinitions();
    auto convertedMaps = convertKessMaps(kessMaps);
    
    for (const auto& map : convertedMaps) {
        project.addMap(map);
    }
    
    return true;
}

bool KessMapConverter::exportToKessFile(const Project& project, const std::string& kessFilepath) {
    KessFileWriter writer;
    
    // Set ECU info
    KessEcuInfo ecuInfo;
    ecuInfo.ecuName = project.ecuName();
    ecuInfo.ecuType = project.ecuName(); // Use ECU name as type
    writer.setEcuInfo(ecuInfo);
    
    // Convert maps
    auto kessMaps = convertToKessMaps(project.maps());
    writer.setMapDefinitions(kessMaps);
    
    // Note: Binary data would need to be set separately
    // This would typically come from the loaded binary file
    
    return writer.writeFile(kessFilepath);
}

std::vector<MapDefinition> KessMapConverter::convertKessMaps(const std::vector<MapDefinition>& kessMaps) {
    std::vector<MapDefinition> result;
    for (const auto& map : kessMaps) {
        result.push_back(convertMap(map));
    }
    return result;
}

std::vector<MapDefinition> KessMapConverter::convertToKessMaps(const std::vector<MapDefinition>& maps) {
    std::vector<MapDefinition> result;
    for (const auto& map : maps) {
        result.push_back(convertMap(map));
    }
    return result;
}

MapDefinition KessMapConverter::convertMap(const MapDefinition& map) {
    // For now, return as-is
    // Actual conversion would adjust scaling, data types, etc. based on KESS format requirements
    return map;
}

} // namespace WinMMM10

