#include "BinaryFile.h"
#include <fstream>
#include <stdexcept>
#include <cstring>

namespace WinMMM10 {

BinaryFile::BinaryFile() = default;

bool BinaryFile::load(const std::string& filepath) {
    clear();
    
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }
    
    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    
    m_data.resize(fileSize);
    if (!file.read(reinterpret_cast<char*>(m_data.data()), fileSize)) {
        m_data.clear();
        return false;
    }
    
    m_filepath = filepath;
    m_loaded = true;
    m_hasChanges = false;
    return true;
}

bool BinaryFile::save(const std::string& filepath) {
    if (!m_loaded || m_data.empty()) {
        return false;
    }
    
    std::ofstream file(filepath, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    
    if (!file.write(reinterpret_cast<const char*>(m_data.data()), m_data.size())) {
        return false;
    }
    
    m_filepath = filepath;
    m_hasChanges = false;
    return true;
}

bool BinaryFile::save() {
    if (m_filepath.empty()) {
        return false;
    }
    return save(m_filepath);
}

void BinaryFile::clear() {
    m_data.clear();
    m_filepath.clear();
    m_loaded = false;
    m_hasChanges = false;
}

uint8_t BinaryFile::readByte(size_t offset) const {
    if (!isValidOffset(offset)) {
        return 0;
    }
    return m_data[offset];
}

int8_t BinaryFile::readInt8(size_t offset) const {
    return static_cast<int8_t>(readByte(offset));
}

uint8_t BinaryFile::readUInt8(size_t offset) const {
    return readByte(offset);
}

int16_t BinaryFile::readInt16(size_t offset, Endianness endian) const {
    if (!isValidOffset(offset + 1)) {
        return 0;
    }
    if (endian == Endianness::Little) {
        return EndiannessConverter::readLittleEndian<int16_t>(m_data.data() + offset);
    } else {
        return EndiannessConverter::readBigEndian<int16_t>(m_data.data() + offset);
    }
}

uint16_t BinaryFile::readUInt16(size_t offset, Endianness endian) const {
    if (!isValidOffset(offset + 1)) {
        return 0;
    }
    if (endian == Endianness::Little) {
        return EndiannessConverter::readLittleEndian<uint16_t>(m_data.data() + offset);
    } else {
        return EndiannessConverter::readBigEndian<uint16_t>(m_data.data() + offset);
    }
}

int32_t BinaryFile::readInt32(size_t offset, Endianness endian) const {
    if (!isValidOffset(offset + 3)) {
        return 0;
    }
    if (endian == Endianness::Little) {
        return EndiannessConverter::readLittleEndian<int32_t>(m_data.data() + offset);
    } else {
        return EndiannessConverter::readBigEndian<int32_t>(m_data.data() + offset);
    }
}

uint32_t BinaryFile::readUInt32(size_t offset, Endianness endian) const {
    if (!isValidOffset(offset + 3)) {
        return 0;
    }
    if (endian == Endianness::Little) {
        return EndiannessConverter::readLittleEndian<uint32_t>(m_data.data() + offset);
    } else {
        return EndiannessConverter::readBigEndian<uint32_t>(m_data.data() + offset);
    }
}

float BinaryFile::readFloat(size_t offset, Endianness endian) const {
    if (!isValidOffset(offset + 3)) {
        return 0.0f;
    }
    if (endian == Endianness::Little) {
        return EndiannessConverter::readLittleEndian<float>(m_data.data() + offset);
    } else {
        return EndiannessConverter::readBigEndian<float>(m_data.data() + offset);
    }
}

bool BinaryFile::writeByte(size_t offset, uint8_t value) {
    if (!isValidOffset(offset)) {
        return false;
    }
    m_data[offset] = value;
    m_hasChanges = true;
    return true;
}

bool BinaryFile::writeInt8(size_t offset, int8_t value) {
    return writeByte(offset, static_cast<uint8_t>(value));
}

bool BinaryFile::writeUInt8(size_t offset, uint8_t value) {
    return writeByte(offset, value);
}

bool BinaryFile::writeInt16(size_t offset, int16_t value, Endianness endian) {
    if (!isValidOffset(offset + 1)) {
        return false;
    }
    if (endian == Endianness::Little) {
        EndiannessConverter::writeLittleEndian<int16_t>(m_data.data() + offset, value);
    } else {
        EndiannessConverter::writeBigEndian<int16_t>(m_data.data() + offset, value);
    }
    m_hasChanges = true;
    return true;
}

bool BinaryFile::writeUInt16(size_t offset, uint16_t value, Endianness endian) {
    if (!isValidOffset(offset + 1)) {
        return false;
    }
    if (endian == Endianness::Little) {
        EndiannessConverter::writeLittleEndian<uint16_t>(m_data.data() + offset, value);
    } else {
        EndiannessConverter::writeBigEndian<uint16_t>(m_data.data() + offset, value);
    }
    m_hasChanges = true;
    return true;
}

bool BinaryFile::writeInt32(size_t offset, int32_t value, Endianness endian) {
    if (!isValidOffset(offset + 3)) {
        return false;
    }
    if (endian == Endianness::Little) {
        EndiannessConverter::writeLittleEndian<int32_t>(m_data.data() + offset, value);
    } else {
        EndiannessConverter::writeBigEndian<int32_t>(m_data.data() + offset, value);
    }
    m_hasChanges = true;
    return true;
}

bool BinaryFile::writeUInt32(size_t offset, uint32_t value, Endianness endian) {
    if (!isValidOffset(offset + 3)) {
        return false;
    }
    if (endian == Endianness::Little) {
        EndiannessConverter::writeLittleEndian<uint32_t>(m_data.data() + offset, value);
    } else {
        EndiannessConverter::writeBigEndian<uint32_t>(m_data.data() + offset, value);
    }
    m_hasChanges = true;
    return true;
}

bool BinaryFile::writeFloat(size_t offset, float value, Endianness endian) {
    if (!isValidOffset(offset + 3)) {
        return false;
    }
    if (endian == Endianness::Little) {
        EndiannessConverter::writeLittleEndian<float>(m_data.data() + offset, value);
    } else {
        EndiannessConverter::writeBigEndian<float>(m_data.data() + offset, value);
    }
    m_hasChanges = true;
    return true;
}

const uint8_t* BinaryFile::at(size_t offset) const {
    if (!isValidOffset(offset)) {
        return nullptr;
    }
    return m_data.data() + offset;
}

uint8_t* BinaryFile::at(size_t offset) {
    if (!isValidOffset(offset)) {
        return nullptr;
    }
    return m_data.data() + offset;
}

std::vector<uint8_t> BinaryFile::readBytes(size_t offset, size_t count) const {
    std::vector<uint8_t> result;
    if (!isValidOffset(offset)) {
        return result;
    }
    
    size_t available = m_data.size() - offset;
    size_t toRead = (count > available) ? available : count;
    
    result.resize(toRead);
    std::memcpy(result.data(), m_data.data() + offset, toRead);
    return result;
}

bool BinaryFile::writeBytes(size_t offset, const std::vector<uint8_t>& bytes) {
    if (bytes.empty()) {
        return true;
    }
    
    if (!isValidOffset(offset) || (offset + bytes.size() > m_data.size())) {
        // Resize if needed
        if (offset + bytes.size() > m_data.size()) {
            m_data.resize(offset + bytes.size());
        }
    }
    
    std::memcpy(m_data.data() + offset, bytes.data(), bytes.size());
    m_hasChanges = true;
    return true;
}

} // namespace WinMMM10

