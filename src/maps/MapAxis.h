#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace WinMMM10 {

enum class AxisType {
    XAxis,
    YAxis,
    ZAxis
};

class MapAxis {
public:
    MapAxis();
    MapAxis(AxisType type, size_t address, size_t count, uint16_t dataType, double factor, double offset);
    
    AxisType type() const { return m_type; }
    void setType(AxisType type) { m_type = type; }
    
    size_t address() const { return m_address; }
    void setAddress(size_t address) { m_address = address; }
    
    size_t count() const { return m_count; }
    void setCount(size_t count) { m_count = count; }
    
    uint16_t dataType() const { return m_dataType; } // 1=uint8, 2=uint16, 3=int16, 4=float
    void setDataType(uint16_t type) { m_dataType = type; }
    
    double factor() const { return m_factor; }
    void setFactor(double factor) { m_factor = factor; }
    
    double offset() const { return m_offset; }
    void setOffset(double offset) { m_offset = offset; }
    
    std::string name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    
    std::string unit() const { return m_unit; }
    void setUnit(const std::string& unit) { m_unit = unit; }

private:
    AxisType m_type{AxisType::XAxis};
    size_t m_address{0};
    size_t m_count{0};
    uint16_t m_dataType{2}; // uint16 default
    double m_factor{1.0};
    double m_offset{0.0};
    std::string m_name;
    std::string m_unit;
};

} // namespace WinMMM10

