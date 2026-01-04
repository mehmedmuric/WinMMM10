#include "PatternAnalyzer.h"
#include "../binary/Endianness.h"
#include <cmath>
#include <algorithm>
#include <numeric>

namespace WinMMM10 {

PatternAnalyzer::PatternResult PatternAnalyzer::analyze2D(const uint8_t* data, size_t offset, size_t count, size_t elementSize) {
    PatternResult result;
    if (!data || count == 0 || elementSize == 0) {
        return result;
    }
    
    std::vector<double> values;
    values.reserve(count);
    
    for (size_t i = 0; i < count; ++i) {
        size_t pos = offset + i * elementSize;
        double value = 0.0;
        
        if (elementSize == 1) {
            value = static_cast<double>(data[pos]);
        } else if (elementSize == 2) {
            value = static_cast<double>(EndiannessConverter::readLittleEndian<uint16_t>(data + pos));
        } else if (elementSize == 4) {
            value = static_cast<double>(EndiannessConverter::readLittleEndian<float>(data + pos));
        }
        
        values.push_back(value);
    }
    
    if (values.empty()) {
        return result;
    }
    
    // Calculate mean
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    result.mean = sum / values.size();
    
    // Calculate variance
    result.variance = calculateVariance(values);
    
    // Check monotonicity
    bool increasing = true;
    bool decreasing = true;
    
    for (size_t i = 1; i < values.size(); ++i) {
        if (values[i] < values[i-1]) {
            increasing = false;
        }
        if (values[i] > values[i-1]) {
            decreasing = false;
        }
    }
    
    result.isMonotonic = increasing || decreasing;
    result.isIncreasing = increasing;
    result.isDecreasing = decreasing;
    
    return result;
}

bool PatternAnalyzer::detectMatrixPattern(const uint8_t* data, size_t offset, size_t rows, size_t cols, size_t elementSize) {
    if (!data || rows == 0 || cols == 0 || elementSize == 0) {
        return false;
    }
    
    // Check if we have enough data
    size_t requiredSize = offset + rows * cols * elementSize;
    // We can't check the full size, so we'll do a basic pattern check
    
    // Check for repeating patterns in rows
    bool hasPattern = true;
    for (size_t row = 1; row < rows && row < 10; ++row) { // Limit check to first 10 rows
        size_t prevRowStart = offset + (row - 1) * cols * elementSize;
        size_t currRowStart = offset + row * cols * elementSize;
        
        // Compare first few elements
        for (size_t col = 0; col < std::min(cols, size_t(5)); ++col) {
            size_t prevPos = prevRowStart + col * elementSize;
            size_t currPos = currRowStart + col * elementSize;
            
            if (elementSize == 2) {
                uint16_t prev = EndiannessConverter::readLittleEndian<uint16_t>(data + prevPos);
                uint16_t curr = EndiannessConverter::readLittleEndian<uint16_t>(data + currPos);
                // Check if values are in reasonable range and not all zeros
                if (prev == 0 && curr == 0) {
                    // Might be padding, continue
                }
            }
        }
    }
    
    return hasPattern;
}

double PatternAnalyzer::calculateVariance(const std::vector<double>& values) {
    if (values.size() <= 1) {
        return 0.0;
    }
    
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    double mean = sum / values.size();
    
    double sqDiff = 0.0;
    for (double value : values) {
        double diff = value - mean;
        sqDiff += diff * diff;
    }
    
    return sqDiff / values.size();
}

} // namespace WinMMM10

