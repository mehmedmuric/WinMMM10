#pragma once

#include <cstdint>
#include <vector>
#include <cstddef>

namespace WinMMM10 {

class PatternAnalyzer {
public:
    struct PatternResult {
        bool isMonotonic{false};
        bool isIncreasing{false};
        bool isDecreasing{false};
        double variance{0.0};
        double mean{0.0};
    };
    
    static PatternResult analyze2D(const uint8_t* data, size_t offset, size_t count, size_t elementSize);
    static bool detectMatrixPattern(const uint8_t* data, size_t offset, size_t rows, size_t cols, size_t elementSize);
    static double calculateVariance(const std::vector<double>& values);
};

} // namespace WinMMM10

