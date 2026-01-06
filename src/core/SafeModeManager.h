#pragma once

#include <string>
#include <cstdint>

namespace WinMMM10 {

class SafeModeManager {
public:
    static SafeModeManager& instance();
    
    // Safe Mode state
    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled);
    
    // Value validation
    enum class ValidationResult {
        Allowed,
        Warning,
        Blocked
    };
    
    struct ValueLimits {
        double hardMin{0.0};
        double hardMax{1000.0};
        double warningMin{0.0};
        double warningMax{1000.0};
    };
    
    ValidationResult validateValue(double value, const ValueLimits& limits, std::string& reason) const;
    
    // Checksum validation
    bool validateChecksum(const uint8_t* data, size_t size, const std::string& algorithm = "CRC32") const;
    
    // ECU signature verification
    std::string computeEcuSignature(const std::string& ecuName, const std::string& softwareVersion, size_t flashSize) const;
    bool verifyEcuSignature(const std::string& currentSignature, const std::string& projectSignature) const;
    
    // Logging
    void logBlock(const std::string& reason) const;
    void logWarning(const std::string& message) const;

private:
    SafeModeManager() = default;
    ~SafeModeManager() = default;
    SafeModeManager(const SafeModeManager&) = delete;
    SafeModeManager& operator=(const SafeModeManager&) = delete;
    
    bool m_enabled{true}; // Default: enabled
};

} // namespace WinMMM10

