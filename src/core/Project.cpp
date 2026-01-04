#include "Project.h"
#include <stdexcept>

namespace WinMMM10 {

Project::Project() = default;

void Project::addMap(const MapDefinition& map) {
    m_maps.push_back(map);
}

void Project::removeMap(size_t index) {
    if (index >= m_maps.size()) {
        return;
    }
    m_maps.erase(m_maps.begin() + index);
}

MapDefinition& Project::getMap(size_t index) {
    if (index >= m_maps.size()) {
        throw std::out_of_range("Map index out of range");
    }
    return m_maps[index];
}

const MapDefinition& Project::getMap(size_t index) const {
    if (index >= m_maps.size()) {
        throw std::out_of_range("Map index out of range");
    }
    return m_maps[index];
}

} // namespace WinMMM10

