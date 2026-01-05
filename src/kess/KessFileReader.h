#pragma once

#include "KessEcuInfo.h"
#include "../binary/BinaryFile.h"
#include "../maps/MapDefinition.h"
#include <string>
#include <vector>
#include <memory>

namespace WinMMM10 {

class KessFileReader {
public:
    KessFileReader();
    ~KessFileReader() = default;
    
    bool readFile(const std::string& filepath);
    bool readFromData(const std::vector<uint8_t>& data);
    
    const KessEcuInfo& getEcuInfo() const { return m_ecuInfo; }
    const std::vector<uint8_t>& getBinaryData() const { return m_binaryData; }
    std::vector<MapDefinition> getMapDefinitions() const { return m_mapDefinitions; }
    
    bool isValid() const { return m_valid; }
    std::string getError() const { return m_error; }

private:
    bool parseKessFile(const std::vector<uint8_t>& data);
    bool parseOriFile(const std::vector<uint8_t>& data);
    bool parseModFile(const std::vector<uint8_t>& data);
    void extractEcuInfo(const std::vector<uint8_t>& data);
    void extractMaps(const std::vector<uint8_t>& data);
    
    KessEcuInfo m_ecuInfo;
    std::vector<uint8_t> m_binaryData;
    std::vector<MapDefinition> m_mapDefinitions;
    bool m_valid{false};
    std::string m_error;
};

} // namespace WinMMM10

