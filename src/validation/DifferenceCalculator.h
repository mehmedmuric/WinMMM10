#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>

namespace WinMMM10 {

class DifferenceCalculator {
public:
    struct Difference {
        size_t offset{0};
        uint8_t originalValue{0};
        uint8_t modifiedValue{0};
    };
    
    DifferenceCalculator();
    ~DifferenceCalculator() = default;
    
    void setOriginal(const uint8_t* data, size_t size);
    void setModified(const uint8_t* data, size_t size);
    
    std::vector<Difference> calculateDifferences();
    size_t differenceCount() const;

private:
    const uint8_t* m_original{nullptr};
    size_t m_originalSize{0};
    const uint8_t* m_modified{nullptr};
    size_t m_modifiedSize{0};
};

} // namespace WinMMM10

