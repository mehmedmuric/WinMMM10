#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>

namespace WinMMM10 {

enum class ChecksumType {
    SimpleSum,
    SimpleSum2Byte,
    CRC16,
    CRC32,
    XOR,
    XOR16,
    Additive,
    Additive16
};

class ChecksumAlgorithm {
public:
    virtual ~ChecksumAlgorithm() = default;
    virtual uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) = 0;
    virtual std::string name() const = 0;
    virtual ChecksumType type() const = 0;
};

class SimpleSumChecksum : public ChecksumAlgorithm {
public:
    uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) override;
    std::string name() const override { return "Simple Sum (8-bit)"; }
    ChecksumType type() const override { return ChecksumType::SimpleSum; }
};

class SimpleSum16Checksum : public ChecksumAlgorithm {
public:
    uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) override;
    std::string name() const override { return "Simple Sum (16-bit)"; }
    ChecksumType type() const override { return ChecksumType::SimpleSum2Byte; }
};

class CRC16Checksum : public ChecksumAlgorithm {
public:
    CRC16Checksum(uint16_t polynomial = 0x1021, uint16_t initialValue = 0xFFFF);
    uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) override;
    std::string name() const override { return "CRC-16"; }
    ChecksumType type() const override { return ChecksumType::CRC16; }

private:
    uint16_t m_polynomial;
    uint16_t m_initialValue;
    uint16_t m_table[256];
    void generateTable();
};

class CRC32Checksum : public ChecksumAlgorithm {
public:
    CRC32Checksum(uint32_t polynomial = 0x04C11DB7);
    uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) override;
    std::string name() const override { return "CRC-32"; }
    ChecksumType type() const override { return ChecksumType::CRC32; }

private:
    uint32_t m_polynomial;
    uint32_t m_table[256];
    void generateTable();
};

class XORChecksum : public ChecksumAlgorithm {
public:
    uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) override;
    std::string name() const override { return "XOR (8-bit)"; }
    ChecksumType type() const override { return ChecksumType::XOR; }
};

class XOR16Checksum : public ChecksumAlgorithm {
public:
    uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) override;
    std::string name() const override { return "XOR (16-bit)"; }
    ChecksumType type() const override { return ChecksumType::XOR16; }
};

class AdditiveChecksum : public ChecksumAlgorithm {
public:
    uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) override;
    std::string name() const override { return "Additive (8-bit)"; }
    ChecksumType type() const override { return ChecksumType::Additive; }
};

class Additive16Checksum : public ChecksumAlgorithm {
public:
    uint32_t calculate(const uint8_t* data, size_t size, size_t startOffset = 0, size_t endOffset = 0) override;
    std::string name() const override { return "Additive (16-bit)"; }
    ChecksumType type() const override { return ChecksumType::Additive16; }
};

class ChecksumManager {
public:
    static ChecksumManager& instance();
    
    std::unique_ptr<ChecksumAlgorithm> createAlgorithm(ChecksumType type);
    std::vector<std::string> getAvailableAlgorithms() const;
    
    uint32_t calculateChecksum(ChecksumType type, const uint8_t* data, size_t size, 
                               size_t startOffset = 0, size_t endOffset = 0);
    bool verifyChecksum(ChecksumType type, const uint8_t* data, size_t size, 
                       size_t checksumOffset, size_t startOffset = 0, size_t endOffset = 0);

private:
    ChecksumManager() = default;
    ~ChecksumManager() = default;
    ChecksumManager(const ChecksumManager&) = delete;
    ChecksumManager& operator=(const ChecksumManager&) = delete;
};

} // namespace WinMMM10

