#pragma once

#include "MapAxis.h"
#include <string>
#include <cstdint>
#include <memory>

namespace WinMMM10 {

enum class MapType {
    Map2D,
    Map3D
};

class MapDefinition {
public:
    MapDefinition();
    ~MapDefinition() = default;
    
    std::string name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    
    size_t address() const { return m_address; }
    void setAddress(size_t address) { m_address = address; }
    
    MapType type() const { return m_type; }
    void setType(MapType type) { m_type = type; }
    
    size_t rows() const { return m_rows; }
    void setRows(size_t rows) { m_rows = rows; }
    
    size_t columns() const { return m_columns; }
    void setColumns(size_t columns) { m_columns = columns; }
    
    uint16_t dataType() const { return m_dataType; } // 1=uint8, 2=uint16, 3=int16, 4=float
    void setDataType(uint16_t type) { m_dataType = type; }
    
    double factor() const { return m_factor; }
    void setFactor(double factor) { m_factor = factor; }
    
    double offset() const { return m_offset; }
    void setOffset(double offset) { m_offset = offset; }
    
    std::string unit() const { return m_unit; }
    void setUnit(const std::string& unit) { m_unit = unit; }
    
    MapAxis& xAxis() { return m_xAxis; }
    const MapAxis& xAxis() const { return m_xAxis; }
    
    MapAxis& yAxis() { return m_yAxis; }
    const MapAxis& yAxis() const { return m_yAxis; }
    
    bool hasXAxis() const { return m_xAxis.count() > 0; }
    bool hasYAxis() const { return m_yAxis.count() > 0; }
    
    size_t dataSize() const;
    size_t totalSize() const;

private:
    std::string m_name;
    size_t m_address{0};
    MapType m_type{MapType::Map2D};
    size_t m_rows{0};
    size_t m_columns{0};
    uint16_t m_dataType{2}; // uint16 default
    double m_factor{1.0};
    double m_offset{0.0};
    std::string m_unit;
    MapAxis m_xAxis;
    MapAxis m_yAxis;
};

} // namespace WinMMM10

