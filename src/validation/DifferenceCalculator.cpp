#include "DifferenceCalculator.h"
#include <algorithm>

namespace WinMMM10 {

DifferenceCalculator::DifferenceCalculator() = default;

void DifferenceCalculator::setOriginal(const uint8_t* data, size_t size) {
    m_original = data;
    m_originalSize = size;
}

void DifferenceCalculator::setModified(const uint8_t* data, size_t size) {
    m_modified = data;
    m_modifiedSize = size;
}

std::vector<DifferenceCalculator::Difference> DifferenceCalculator::calculateDifferences() {
    std::vector<Difference> differences;
    
    if (!m_original || !m_modified) {
        return differences;
    }
    
    size_t minSize = std::min(m_originalSize, m_modifiedSize);
    
    for (size_t i = 0; i < minSize; ++i) {
        if (m_original[i] != m_modified[i]) {
            Difference diff;
            diff.offset = i;
            diff.originalValue = m_original[i];
            diff.modifiedValue = m_modified[i];
            differences.push_back(diff);
        }
    }
    
    return differences;
}

size_t DifferenceCalculator::differenceCount() const {
    if (!m_original || !m_modified) {
        return 0;
    }
    
    size_t count = 0;
    size_t minSize = std::min(m_originalSize, m_modifiedSize);
    
    for (size_t i = 0; i < minSize; ++i) {
        if (m_original[i] != m_modified[i]) {
            ++count;
        }
    }
    
    return count;
}

} // namespace WinMMM10

