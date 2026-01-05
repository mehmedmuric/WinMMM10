#include "KessFileWriter.h"
#include <fstream>
#include <cstring>

namespace WinMMM10 {

KessFileWriter::KessFileWriter() {
}

bool KessFileWriter::writeFile(const std::string& filepath) {
    std::vector<uint8_t> data = writeToData();
    
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}

std::vector<uint8_t> KessFileWriter::writeToData() {
    return buildKessFile();
}

std::vector<uint8_t> KessFileWriter::buildKessFile() {
    std::vector<uint8_t> output;
    
    // Write header
    writeHeader(output);
    
    // Write binary data
    writeBinaryData(output);
    
    // Write map definitions (if any)
    writeMapDefinitions(output);
    
    return output;
}

void KessFileWriter::writeHeader(std::vector<uint8_t>& output) {
    // Write a simple header structure
    // Actual format would depend on KESS specification
    
    // Header size placeholder
    uint32_t headerSize = 256;
    output.resize(headerSize, 0);
    
    // Write ECU name (first 64 bytes)
    size_t nameLen = std::min(m_ecuInfo.ecuName.size(), size_t(63));
    std::memcpy(output.data(), m_ecuInfo.ecuName.c_str(), nameLen);
    
    // Write ECU type (bytes 64-127)
    if (output.size() > 64) {
        nameLen = std::min(m_ecuInfo.ecuType.size(), size_t(63));
        std::memcpy(output.data() + 64, m_ecuInfo.ecuType.c_str(), nameLen);
    }
    
    // Write file size (bytes 128-131)
    if (output.size() > 131) {
        uint32_t totalSize = static_cast<uint32_t>(m_binaryData.size() + headerSize);
        std::memcpy(output.data() + 128, &totalSize, sizeof(uint32_t));
    }
}

void KessFileWriter::writeBinaryData(std::vector<uint8_t>& output) {
    // Append binary data after header
    output.insert(output.end(), m_binaryData.begin(), m_binaryData.end());
}

void KessFileWriter::writeMapDefinitions(std::vector<uint8_t>& output) {
    // Map definitions could be written as metadata
    // For now, this is a placeholder
    // Actual implementation would serialize MapDefinition objects
}

} // namespace WinMMM10

