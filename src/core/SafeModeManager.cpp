#include "SafeModeManager.h"
#include "../binary/Checksum.h"
#include <sstream>
#include <iomanip>
#include <functional>
#include <iostream>

namespace WinMMM10 {

SafeModeManager& SafeModeManager::instance() {
    static SafeModeManager instance;
    return instance;
}

void SafeModeManager::setEnabled(bool enabled) {
    if (m_enabled != enabled) {
        m_enabled = enabled;
        // Log to standard output (UI layer will handle Qt logging)
        std::cout << "[SAFE MODE] " << (enabled ? "ENABLED" : "DISABLED") << std::endl;
    }
}

SafeModeManager::ValidationResult SafeModeManager::validateValue(
    double value, 
    const ValueLimits& limits, 
    std::string& reason) const {
    
    if (!m_enabled) {
        return ValidationResult::Allowed;
    }
    
    // Check hard limits (block)
    if (value < limits.hardMin) {
        reason = "Value " + std::to_string(value) + " below hard minimum " + std::to_string(limits.hardMin);
        return ValidationResult::Blocked;
    }
    
    if (value > limits.hardMax) {
        reason = "Value " + std::to_string(value) + " above hard maximum " + std::to_string(limits.hardMax);
        return ValidationResult::Blocked;
    }
    
    // Check warning limits (allow but warn)
    if (value < limits.warningMin || value > limits.warningMax) {
        reason = "Value " + std::to_string(value) + " outside warning range [" + 
                 std::to_string(limits.warningMin) + ", " + std::to_string(limits.warningMax) + "]";
        return ValidationResult::Warning;
    }
    
    return ValidationResult::Allowed;
}

bool SafeModeManager::validateChecksum(
    const uint8_t* data, 
    size_t size, 
    const std::string& algorithm) const {
    
    if (!m_enabled) {
        return true; // Skip validation when safe mode is off
    }
    
    if (!data || size == 0) {
        logBlock("Checksum validation failed: Invalid data");
        return false;
    }
    
    // Use CRC32 as default checksum algorithm
    ChecksumType type = ChecksumType::CRC32;
    if (algorithm == "CRC16") {
        type = ChecksumType::CRC16;
    } else if (algorithm == "SimpleSum") {
        type = ChecksumType::SimpleSum;
    }
    
    // For now, we'll calculate checksum and log it
    // In a real implementation, you'd compare against stored checksum
    uint32_t calculated = ChecksumManager::instance().calculateChecksum(type, data, size);
    
    // Log to standard output (UI layer will handle Qt logging)
    std::ostringstream oss;
    oss << "[SAFE MODE] Checksum calculated: 0x" << std::hex << std::uppercase << calculated;
    std::cout << oss.str() << std::endl;
    
    // TODO: Compare with stored checksum if available
    // For now, we'll allow it but log
    return true;
}

std::string SafeModeManager::computeEcuSignature(
    const std::string& ecuName,
    const std::string& softwareVersion,
    size_t flashSize) const {
    
    // Simple hash-based signature
    std::hash<std::string> hasher;
    size_t hash = hasher(ecuName + "|" + softwareVersion + "|" + std::to_string(flashSize));
    
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << hash;
    return oss.str();
}

bool SafeModeManager::verifyEcuSignature(
    const std::string& currentSignature,
    const std::string& projectSignature) const {
    
    if (!m_enabled) {
        return true; // Skip validation when safe mode is off
    }
    
    bool match = (currentSignature == projectSignature);
    
    if (!match) {
        logBlock("ECU signature mismatch. Current: " + currentSignature + 
                 ", Project: " + projectSignature);
    }
    
    return match;
}

void SafeModeManager::logBlock(const std::string& reason) const {
    std::cerr << "[SAFE MODE BLOCK] " << reason << std::endl;
}

void SafeModeManager::logWarning(const std::string& message) const {
    std::cerr << "[SAFE MODE WARNING] " << message << std::endl;
}

} // namespace WinMMM10

