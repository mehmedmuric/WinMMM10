#pragma once

#include "KessEcuInfo.h"
#include "../binary/BinaryFile.h"
#include "../maps/MapDefinition.h"
#include <string>
#include <vector>

namespace WinMMM10 {

class KessFileWriter {
public:
    KessFileWriter();
    ~KessFileWriter() = default;
    
    void setEcuInfo(const KessEcuInfo& info) { m_ecuInfo = info; }
    void setBinaryData(const std::vector<uint8_t>& data) { m_binaryData = data; }
    void setMapDefinitions(const std::vector<MapDefinition>& maps) { m_mapDefinitions = maps; }
    
    bool writeFile(const std::string& filepath);
    std::vector<uint8_t> writeToData();

private:
    std::vector<uint8_t> buildKessFile();
    void writeHeader(std::vector<uint8_t>& output);
    void writeBinaryData(std::vector<uint8_t>& output);
    void writeMapDefinitions(std::vector<uint8_t>& output);
    
    KessEcuInfo m_ecuInfo;
    std::vector<uint8_t> m_binaryData;
    std::vector<MapDefinition> m_mapDefinitions;
};

} // namespace WinMMM10

