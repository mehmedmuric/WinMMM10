#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace WinMMM10 {

class ScalingEngine {
public:
    // Raw to Physical conversions
    static double rawToPhysical(uint16_t rawValue, double factor, double offset) {
        return static_cast<double>(rawValue) * factor + offset;
    }
    
    static double rawToPhysical(int16_t rawValue, double factor, double offset) {
        return static_cast<double>(rawValue) * factor + offset;
    }
    
    static double rawToPhysical(uint8_t rawValue, double factor, double offset) {
        return static_cast<double>(rawValue) * factor + offset;
    }
    
    static double rawToPhysical(float rawValue, double factor, double offset) {
        return static_cast<double>(rawValue) * factor + offset;
    }
    
    // Physical to Raw conversions - use different names to avoid overload conflicts
    static uint16_t physicalToRawU16(double physicalValue, double factor, double offset) {
        double raw = (physicalValue - offset) / factor;
        if (raw < 0) return 0;
        if (raw > 65535) return 65535;
        return static_cast<uint16_t>(raw + 0.5);
    }
    
    static int16_t physicalToRawI16(double physicalValue, double factor, double offset) {
        double raw = (physicalValue - offset) / factor;
        if (raw < -32768) return -32768;
        if (raw > 32767) return 32767;
        return static_cast<int16_t>(raw + (raw >= 0 ? 0.5 : -0.5));
    }
    
    static uint8_t physicalToRawU8(double physicalValue, double factor, double offset) {
        double raw = (physicalValue - offset) / factor;
        if (raw < 0) return 0;
        if (raw > 255) return 255;
        return static_cast<uint8_t>(raw + 0.5);
    }
    
    static float physicalToRawFloat(double physicalValue, double factor, double offset) {
        return static_cast<float>((physicalValue - offset) / factor);
    }
    
    // Template helper for type-based dispatch
    template<typename T>
    static T physicalToRaw(double physicalValue, double factor, double offset) {
        if constexpr (std::is_same_v<T, uint16_t>) {
            return physicalToRawU16(physicalValue, factor, offset);
        } else if constexpr (std::is_same_v<T, int16_t>) {
            return physicalToRawI16(physicalValue, factor, offset);
        } else if constexpr (std::is_same_v<T, uint8_t>) {
            return physicalToRawU8(physicalValue, factor, offset);
        } else if constexpr (std::is_same_v<T, float>) {
            return physicalToRawFloat(physicalValue, factor, offset);
        } else {
            static_assert(std::is_same_v<T, void>, "Unsupported type for physicalToRaw");
        }
    }
};

} // namespace WinMMM10

