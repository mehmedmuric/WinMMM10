#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

namespace WinMMM10 {

struct ValidationWarning {
    std::string message;
    size_t address{0};
    double oldValue{0.0};
    double newValue{0.0};
    double changePercent{0.0};
};

class Validator {
public:
    Validator();
    ~Validator() = default;
    
    void setMaxChangePercent(double percent) { m_maxChangePercent = percent; }
    void setMinValue(double value) { m_minValue = value; }
    void setMaxValue(double value) { m_maxValue = value; }
    
    std::vector<ValidationWarning> validateChange(
        size_t address,
        double oldValue,
        double newValue
    );

private:
    double m_maxChangePercent{50.0};
    double m_minValue{-1000000.0};
    double m_maxValue{1000000.0};
};

} // namespace WinMMM10

