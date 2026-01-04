#include "MapAxis.h"

namespace WinMMM10 {

MapAxis::MapAxis() = default;

MapAxis::MapAxis(AxisType type, size_t address, size_t count, uint16_t dataType, double factor, double offset)
    : m_type(type)
    , m_address(address)
    , m_count(count)
    , m_dataType(dataType)
    , m_factor(factor)
    , m_offset(offset)
{
}

} // namespace WinMMM10

