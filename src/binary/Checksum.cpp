#include "Checksum.h"
#include "Endianness.h"
#include <algorithm>
#include <numeric>

namespace WinMMM10 {

uint32_t SimpleSumChecksum::calculate(const uint8_t* data, size_t size, size_t startOffset, size_t endOffset) {
    if (!data || size == 0) return 0;
    
    size_t start = (startOffset > 0) ? startOffset : 0;
    size_t end = (endOffset > 0 && endOffset < size) ? endOffset : size;
    
    uint32_t sum = 0;
    for (size_t i = start; i < end; ++i) {
        sum += data[i];
    }
    return sum & 0xFF;
}

uint32_t SimpleSum16Checksum::calculate(const uint8_t* data, size_t size, size_t startOffset, size_t endOffset) {
    if (!data || size < 2) return 0;
    
    size_t start = (startOffset > 0) ? startOffset : 0;
    size_t end = (endOffset > 0 && endOffset < size) ? endOffset : size;
    
    // Align to 2-byte boundary
    if (start % 2 != 0) start++;
    if (end % 2 != 0) end--;
    
    uint32_t sum = 0;
    for (size_t i = start; i < end; i += 2) {
        if (i + 1 < size) {
            uint16_t value = EndiannessConverter::readLittleEndian<uint16_t>(data + i);
            sum += value;
        }
    }
    return sum & 0xFFFF;
}

CRC16Checksum::CRC16Checksum(uint16_t polynomial, uint16_t initialValue)
    : m_polynomial(polynomial)
    , m_initialValue(initialValue)
{
    generateTable();
}

void CRC16Checksum::generateTable() {
    for (uint16_t i = 0; i < 256; ++i) {
        uint16_t crc = i << 8;
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ m_polynomial;
            } else {
                crc <<= 1;
            }
        }
        m_table[i] = crc;
    }
}

uint32_t CRC16Checksum::calculate(const uint8_t* data, size_t size, size_t startOffset, size_t endOffset) {
    if (!data || size == 0) return m_initialValue;
    
    size_t start = (startOffset > 0) ? startOffset : 0;
    size_t end = (endOffset > 0 && endOffset < size) ? endOffset : size;
    
    uint16_t crc = m_initialValue;
    for (size_t i = start; i < end; ++i) {
        uint8_t byte = data[i];
        uint8_t index = (crc >> 8) ^ byte;
        crc = (crc << 8) ^ m_table[index];
    }
    return crc;
}

CRC32Checksum::CRC32Checksum(uint32_t polynomial)
    : m_polynomial(polynomial)
{
    generateTable();
}

void CRC32Checksum::generateTable() {
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (int j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ m_polynomial;
            } else {
                crc >>= 1;
            }
        }
        m_table[i] = crc;
    }
}

uint32_t CRC32Checksum::calculate(const uint8_t* data, size_t size, size_t startOffset, size_t endOffset) {
    if (!data || size == 0) return 0xFFFFFFFF;
    
    size_t start = (startOffset > 0) ? startOffset : 0;
    size_t end = (endOffset > 0 && endOffset < size) ? endOffset : size;
    
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = start; i < end; ++i) {
        uint8_t byte = data[i];
        uint8_t index = (crc ^ byte) & 0xFF;
        crc = (crc >> 8) ^ m_table[index];
    }
    return ~crc;
}

uint32_t XORChecksum::calculate(const uint8_t* data, size_t size, size_t startOffset, size_t endOffset) {
    if (!data || size == 0) return 0;
    
    size_t start = (startOffset > 0) ? startOffset : 0;
    size_t end = (endOffset > 0 && endOffset < size) ? endOffset : size;
    
    uint8_t result = 0;
    for (size_t i = start; i < end; ++i) {
        result ^= data[i];
    }
    return result;
}

uint32_t XOR16Checksum::calculate(const uint8_t* data, size_t size, size_t startOffset, size_t endOffset) {
    if (!data || size < 2) return 0;
    
    size_t start = (startOffset > 0) ? startOffset : 0;
    size_t end = (endOffset > 0 && endOffset < size) ? endOffset : size;
    
    if (start % 2 != 0) start++;
    if (end % 2 != 0) end--;
    
    uint16_t result = 0;
    for (size_t i = start; i < end; i += 2) {
        if (i + 1 < size) {
            uint16_t value = EndiannessConverter::readLittleEndian<uint16_t>(data + i);
            result ^= value;
        }
    }
    return result;
}

uint32_t AdditiveChecksum::calculate(const uint8_t* data, size_t size, size_t startOffset, size_t endOffset) {
    if (!data || size == 0) return 0;
    
    size_t start = (startOffset > 0) ? startOffset : 0;
    size_t end = (endOffset > 0 && endOffset < size) ? endOffset : size;
    
    uint32_t sum = 0;
    for (size_t i = start; i < end; ++i) {
        sum = (sum + data[i]) % 256;
    }
    return sum;
}

uint32_t Additive16Checksum::calculate(const uint8_t* data, size_t size, size_t startOffset, size_t endOffset) {
    if (!data || size < 2) return 0;
    
    size_t start = (startOffset > 0) ? startOffset : 0;
    size_t end = (endOffset > 0 && endOffset < size) ? endOffset : size;
    
    if (start % 2 != 0) start++;
    if (end % 2 != 0) end--;
    
    uint32_t sum = 0;
    for (size_t i = start; i < end; i += 2) {
        if (i + 1 < size) {
            uint16_t value = EndiannessConverter::readLittleEndian<uint16_t>(data + i);
            sum = (sum + value) % 65536;
        }
    }
    return sum;
}

ChecksumManager& ChecksumManager::instance() {
    static ChecksumManager instance;
    return instance;
}

std::unique_ptr<ChecksumAlgorithm> ChecksumManager::createAlgorithm(ChecksumType type) {
    switch (type) {
        case ChecksumType::SimpleSum:
            return std::make_unique<SimpleSumChecksum>();
        case ChecksumType::SimpleSum2Byte:
            return std::make_unique<SimpleSum16Checksum>();
        case ChecksumType::CRC16:
            return std::make_unique<CRC16Checksum>();
        case ChecksumType::CRC32:
            return std::make_unique<CRC32Checksum>();
        case ChecksumType::XOR:
            return std::make_unique<XORChecksum>();
        case ChecksumType::XOR16:
            return std::make_unique<XOR16Checksum>();
        case ChecksumType::Additive:
            return std::make_unique<AdditiveChecksum>();
        case ChecksumType::Additive16:
            return std::make_unique<Additive16Checksum>();
        default:
            return std::make_unique<SimpleSumChecksum>();
    }
}

std::vector<std::string> ChecksumManager::getAvailableAlgorithms() const {
    return {
        "Simple Sum (8-bit)",
        "Simple Sum (16-bit)",
        "CRC-16",
        "CRC-32",
        "XOR (8-bit)",
        "XOR (16-bit)",
        "Additive (8-bit)",
        "Additive (16-bit)"
    };
}

uint32_t ChecksumManager::calculateChecksum(ChecksumType type, const uint8_t* data, size_t size,
                                           size_t startOffset, size_t endOffset) {
    auto algorithm = createAlgorithm(type);
    return algorithm->calculate(data, size, startOffset, endOffset);
}

bool ChecksumManager::verifyChecksum(ChecksumType type, const uint8_t* data, size_t size,
                                    size_t checksumOffset, size_t startOffset, size_t endOffset) {
    if (checksumOffset >= size) return false;
    
    uint32_t calculated = calculateChecksum(type, data, size, startOffset, endOffset);
    
    // Read stored checksum (assume 2 bytes for most, 4 bytes for CRC32)
    uint32_t stored = 0;
    if (type == ChecksumType::CRC32) {
        if (checksumOffset + 4 <= size) {
            stored = EndiannessConverter::readLittleEndian<uint32_t>(data + checksumOffset);
        }
    } else {
        if (checksumOffset + 2 <= size) {
            stored = EndiannessConverter::readLittleEndian<uint16_t>(data + checksumOffset);
        } else if (checksumOffset + 1 <= size) {
            stored = data[checksumOffset];
        }
    }
    
    return calculated == stored;
}

} // namespace WinMMM10

