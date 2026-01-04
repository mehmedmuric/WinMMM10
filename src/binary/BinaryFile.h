#pragma once

#include "MemoryMapper.h"
#include "Endianness.h"
#include <memory>
#include <vector>
#include <cstdint>
#include <string>

namespace WinMMM10 {

class BinaryFile {
public:
    BinaryFile();
    ~BinaryFile() = default;
    
    bool load(const std::string& filepath);
    bool save(const std::string& filepath);
    bool save();
    
    bool isLoaded() const { return m_loaded; }
    size_t size() const { return m_data.size(); }
    const std::string& filepath() const { return m_filepath; }
    
    uint8_t readByte(size_t offset) const;
    int8_t readInt8(size_t offset) const;
    uint8_t readUInt8(size_t offset) const;
    int16_t readInt16(size_t offset, Endianness endian = Endianness::Little) const;
    uint16_t readUInt16(size_t offset, Endianness endian = Endianness::Little) const;
    int32_t readInt32(size_t offset, Endianness endian = Endianness::Little) const;
    uint32_t readUInt32(size_t offset, Endianness endian = Endianness::Little) const;
    float readFloat(size_t offset, Endianness endian = Endianness::Little) const;
    
    bool writeByte(size_t offset, uint8_t value);
    bool writeInt8(size_t offset, int8_t value);
    bool writeUInt8(size_t offset, uint8_t value);
    bool writeInt16(size_t offset, int16_t value, Endianness endian = Endianness::Little);
    bool writeUInt16(size_t offset, uint16_t value, Endianness endian = Endianness::Little);
    bool writeInt32(size_t offset, int32_t value, Endianness endian = Endianness::Little);
    bool writeUInt32(size_t offset, uint32_t value, Endianness endian = Endianness::Little);
    bool writeFloat(size_t offset, float value, Endianness endian = Endianness::Little);
    
    const uint8_t* data() const { return m_data.data(); }
    uint8_t* data() { return m_data.data(); }
    
    const uint8_t* at(size_t offset) const;
    uint8_t* at(size_t offset);
    
    bool isValidOffset(size_t offset) const {
        return offset < m_data.size();
    }
    
    std::vector<uint8_t> readBytes(size_t offset, size_t count) const;
    bool writeBytes(size_t offset, const std::vector<uint8_t>& bytes);
    
    void clear();
    bool hasChanges() const { return m_hasChanges; }
    void markChanged() { m_hasChanges = true; }
    void markSaved() { m_hasChanges = false; }

private:
    std::vector<uint8_t> m_data;
    std::string m_filepath;
    bool m_loaded{false};
    bool m_hasChanges{false};
};

} // namespace WinMMM10

