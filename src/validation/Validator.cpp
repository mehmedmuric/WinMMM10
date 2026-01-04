#include "Validator.h"
#include <cmath>
#include <algorithm>

namespace WinMMM10 {

Validator::Validator() = default;

std::vector<ValidationWarning> Validator::validateChange(
    size_t address,
    double oldValue,
    double newValue)
{
    std::vector<ValidationWarning> warnings;
    
    // Check value range
    if (newValue < m_minValue) {
        ValidationWarning warning;
        warning.address = address;
        warning.oldValue = oldValue;
        warning.newValue = newValue;
        warning.message = "Value below minimum (" + std::to_string(m_minValue) + ")";
        warnings.push_back(warning);
    }
    
    if (newValue > m_maxValue) {
        ValidationWarning warning;
        warning.address = address;
        warning.oldValue = oldValue;
        warning.newValue = newValue;
        warning.message = "Value above maximum (" + std::to_string(m_maxValue) + ")";
        warnings.push_back(warning);
    }
    
    // Check change percentage
    if (std::abs(oldValue) > 0.0001) {
        double changePercent = std::abs((newValue - oldValue) / oldValue) * 100.0;
        if (changePercent > m_maxChangePercent) {
            ValidationWarning warning;
            warning.address = address;
            warning.oldValue = oldValue;
            warning.newValue = newValue;
            warning.changePercent = changePercent;
            warning.message = "Change exceeds " + std::to_string(m_maxChangePercent) + "% (" + 
                            std::to_string(changePercent) + "%)";
            warnings.push_back(warning);
        }
    }
    
    return warnings;
}

} // namespace WinMMM10

