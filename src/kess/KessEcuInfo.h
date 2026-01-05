#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace WinMMM10 {

struct KessEcuInfo {
    std::string ecuName;
    std::string ecuType;
    std::string softwareVersion;
    std::string hardwareVersion;
    std::string calibrationId;
    std::string checksum;
    uint32_t fileSize{0};
    std::vector<uint8_t> headerData;
    
    KessEcuInfo() = default;
};

} // namespace WinMMM10

