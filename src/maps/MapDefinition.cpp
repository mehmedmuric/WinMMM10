#include "MapDefinition.h"

namespace WinMMM10 {

MapDefinition::MapDefinition() {
    m_xAxis.setType(AxisType::XAxis);
    m_yAxis.setType(AxisType::YAxis);
}

size_t MapDefinition::dataSize() const {
    switch (m_dataType) {
        case 1: return 1; // uint8
        case 2: return 2; // uint16
        case 3: return 2; // int16
        case 4: return 4; // float
        default: return 2;
    }
}

size_t MapDefinition::totalSize() const {
    size_t size = m_rows * m_columns * dataSize();
    if (hasXAxis()) {
        size += m_xAxis.count() * (m_xAxis.dataType() == 1 ? 1 : (m_xAxis.dataType() == 4 ? 4 : 2));
    }
    if (hasYAxis() && m_type == MapType::Map3D) {
        size += m_yAxis.count() * (m_yAxis.dataType() == 1 ? 1 : (m_yAxis.dataType() == 4 ? 4 : 2));
    }
    return size;
}

} // namespace WinMMM10

