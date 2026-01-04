#pragma once

#include "MapDefinition.h"
#include <vector>
#include <cstdint>

namespace WinMMM10 {

class Map2D {
public:
    Map2D();
    explicit Map2D(const MapDefinition& definition);
    ~Map2D() = default;
    
    const MapDefinition& definition() const { return m_definition; }
    MapDefinition& definition() { return m_definition; }
    
    void loadFromBinary(const uint8_t* data, size_t dataSize);
    void writeToBinary(uint8_t* data, size_t dataSize) const;
    
    uint16_t getRawValue(size_t index) const;
    void setRawValue(size_t index, uint16_t value);
    
    double getPhysicalValue(size_t index) const;
    void setPhysicalValue(size_t index, double value);
    
    double getXAxisValue(size_t index) const;
    void setXAxisValue(size_t index, double value);
    
    size_t pointCount() const { return m_data.size(); }
    
    std::vector<uint16_t>& rawData() { return m_data; }
    const std::vector<uint16_t>& rawData() const { return m_data; }
    
    std::vector<double>& xAxisData() { return m_xAxis; }
    const std::vector<double>& xAxisData() const { return m_xAxis; }

private:
    MapDefinition m_definition;
    std::vector<uint16_t> m_data;
    std::vector<double> m_xAxis;
};

} // namespace WinMMM10

