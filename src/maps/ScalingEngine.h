#pragma once

#include <cstdint>
#include <cstddef>

namespace WinMMM10 {

class ScalingEngine {
public:
    static double rawToPhysical(uint16_t rawValue, double factor, double offset) {
        return static_cast<double>(rawValue) * factor + offset;
    }
    
    static uint16_t physicalToRaw(double physicalValue, double factor, double offset) {
        double raw = (physicalValue - offset) / factor;
        if (raw < 0) return 0;
        if (raw > 65535) return 65535;
        return static_cast<uint16_t>(raw + 0.5);
    }
    
    static double rawToPhysical(int16_t rawValue, double factor, double offset) {
        return static_cast<double>(rawValue) * factor + offset;
    }
    
    static int16_t physicalToRaw(double physicalValue, double factor, double offset) {
        double raw = (physicalValue - offset) / factor;
        if (raw < -32768) return -32768;
        if (raw > 32767) return 32767;
        return static_cast<int16_t>(raw + (raw >= 0 ? 0.5 : -0.5));
    }
    
    static double rawToPhysical(uint8_t rawValue, double factor, double offset) {
        return static_cast<double>(rawValue) * factor + offset;
    }
    
    static uint8_t physicalToRaw(double physicalValue, double factor, double offset) {
        double raw = (physicalValue - offset) / factor;
        if (raw < 0) return 0;
        if (raw > 255) return 255;
        return static_cast<uint8_t>(raw + 0.5);
    }
    
    static double rawToPhysical(float rawValue, double factor, double offset) {
        return static_cast<double>(rawValue) * factor + offset;
    }
    
    static float physicalToRaw(double physicalValue, double factor, double offset) {
        return static_cast<float>((physicalValue - offset) / factor);
    }
};

} // namespace WinMMM10

