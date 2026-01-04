#pragma once

#include <cstdint>
#include <algorithm>

namespace WinMMM10 {

enum class Endianness {
    Little,
    Big
};

class EndiannessConverter {
public:
    static Endianness systemEndianness();
    
    template<typename T>
    static T swapBytes(T value) {
        uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);
        std::reverse(bytes, bytes + sizeof(T));
        return value;
    }
    
    template<typename T>
    static T toLittleEndian(T value, Endianness from) {
        if (from == Endianness::Big && systemEndianness() == Endianness::Little) {
            return swapBytes(value);
        }
        if (from == Endianness::Little && systemEndianness() == Endianness::Big) {
            return swapBytes(value);
        }
        return value;
    }
    
    template<typename T>
    static T toBigEndian(T value, Endianness from) {
        if (from == Endianness::Little && systemEndianness() == Endianness::Big) {
            return swapBytes(value);
        }
        if (from == Endianness::Big && systemEndianness() == Endianness::Little) {
            return swapBytes(value);
        }
        return value;
    }
    
    template<typename T>
    static T readLittleEndian(const uint8_t* data) {
        T value{};
        std::memcpy(&value, data, sizeof(T));
        if (systemEndianness() == Endianness::Big) {
            value = swapBytes(value);
        }
        return value;
    }
    
    template<typename T>
    static T readBigEndian(const uint8_t* data) {
        T value{};
        std::memcpy(&value, data, sizeof(T));
        if (systemEndianness() == Endianness::Little) {
            value = swapBytes(value);
        }
        return value;
    }
    
    template<typename T>
    static void writeLittleEndian(uint8_t* data, T value) {
        if (systemEndianness() == Endianness::Big) {
            value = swapBytes(value);
        }
        std::memcpy(data, &value, sizeof(T));
    }
    
    template<typename T>
    static void writeBigEndian(uint8_t* data, T value) {
        if (systemEndianness() == Endianness::Little) {
            value = swapBytes(value);
        }
        std::memcpy(data, &value, sizeof(T));
    }
};

inline Endianness EndiannessConverter::systemEndianness() {
    uint16_t test = 0x0102;
    return (*reinterpret_cast<uint8_t*>(&test) == 0x02) ? Endianness::Little : Endianness::Big;
}

} // namespace WinMMM10

