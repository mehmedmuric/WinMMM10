#pragma once

#include "MapDefinition.h"
#include <vector>
#include <cstdint>

namespace WinMMM10 {

class Map3D {
public:
    Map3D();
    explicit Map3D(const MapDefinition& definition);
    ~Map3D() = default;
    
    const MapDefinition& definition() const { return m_definition; }
    MapDefinition& definition() { return m_definition; }
    
    void loadFromBinary(const uint8_t* data, size_t dataSize);
    void writeToBinary(uint8_t* data, size_t dataSize) const;
    
    uint16_t getRawValue(size_t row, size_t col) const;
    void setRawValue(size_t row, size_t col, uint16_t value);
    
    double getPhysicalValue(size_t row, size_t col) const;
    void setPhysicalValue(size_t row, size_t col, double value);
    
    double getXAxisValue(size_t index) const;
    void setXAxisValue(size_t index, double value);
    
    double getYAxisValue(size_t index) const;
    void setYAxisValue(size_t index, double value);
    
    size_t rows() const { return m_rows; }
    size_t columns() const { return m_columns; }
    
    std::vector<uint16_t>& rawData() { return m_data; }
    const std::vector<uint16_t>& rawData() const { return m_data; }
    
    std::vector<double>& xAxisData() { return m_xAxis; }
    const std::vector<double>& xAxisData() const { return m_xAxis; }
    
    std::vector<double>& yAxisData() { return m_yAxis; }
    const std::vector<double>& yAxisData() const { return m_yAxis; }

private:
    MapDefinition m_definition;
    size_t m_rows{0};
    size_t m_columns{0};
    std::vector<uint16_t> m_data;
    std::vector<double> m_xAxis;
    std::vector<double> m_yAxis;
    
    size_t indexOf(size_t row, size_t col) const {
        return row * m_columns + col;
    }
};

} // namespace WinMMM10

