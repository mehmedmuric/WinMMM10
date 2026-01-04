#include "MapDetector.h"
#include "PatternAnalyzer.h"
#include "binary/Endianness.h"
#include <algorithm>
#include <cmath>

namespace WinMMM10 {

MapDetector::MapDetector() = default;

void MapDetector::setBinaryData(const uint8_t* data, size_t size) {
    m_data = data;
    m_size = size;
}

std::vector<MapCandidate> MapDetector::detectMaps(size_t minAddress, size_t maxAddress) {
    std::vector<MapCandidate> candidates;
    
    if (!m_data || m_size == 0) {
        return candidates;
    }
    
    if (maxAddress == 0 || maxAddress > m_size) {
        maxAddress = m_size;
    }
    
    // Enhanced scanning with multiple passes
    const size_t stepSize = 2; // Step by 2 bytes
    const size_t minMapSize = 16; // Minimum map size
    
    // First pass: Quick scan with lower threshold
    for (size_t offset = minAddress; offset < maxAddress - minMapSize; offset += stepSize * 2) {
        // Try 2D map
        MapCandidate candidate2D = detect2DMap(offset);
        if (candidate2D.confidence > 0.2 && isValidMapAddress(offset, candidate2D)) {
            candidates.push_back(candidate2D);
        }
        
        // Try 3D map
        if (offset + 256 < maxAddress) {
            MapCandidate candidate3D = detect3DMap(offset);
            if (candidate3D.confidence > 0.2 && isValidMapAddress(offset, candidate3D)) {
                candidates.push_back(candidate3D);
            }
        }
    }
    
    // Second pass: Refine candidates with higher threshold
    std::vector<MapCandidate> refined;
    for (const auto& candidate : candidates) {
        MapCandidate refinedCandidate = (candidate.type == MapType::Map2D) ? 
                                       detect2DMap(candidate.address) : 
                                       detect3DMap(candidate.address);
        
        // Additional validation
        if (refinedCandidate.confidence > 0.4 && isValidMapAddress(candidate.address, refinedCandidate)) {
            refined.push_back(refinedCandidate);
        }
    }
    
    // Remove duplicates and sort by confidence
    std::sort(refined.begin(), refined.end(), 
        [](const MapCandidate& a, const MapCandidate& b) {
            return a.confidence > b.confidence;
        });
    
    // Remove overlapping candidates (keep highest confidence)
    std::vector<MapCandidate> filtered;
    for (const auto& candidate : refined) {
        bool overlaps = false;
        for (auto& existing : filtered) {
            size_t candidateEnd = candidate.address + candidate.rows * candidate.columns * 2;
            size_t existingEnd = existing.address + existing.rows * existing.columns * 2;
            
            if ((candidate.address >= existing.address && candidate.address < existingEnd) ||
                (existing.address >= candidate.address && existing.address < candidateEnd)) {
                // Keep the one with higher confidence
                if (candidate.confidence > existing.confidence) {
                    existing = candidate;
                }
                overlaps = true;
                break;
            }
        }
        
        if (!overlaps) {
            filtered.push_back(candidate);
        }
    }
    
    // Limit to top 50 candidates
    if (filtered.size() > 50) {
        filtered.resize(50);
    }
    
    return filtered;
}

MapCandidate MapDetector::detect2DMap(size_t offset) {
    MapCandidate candidate;
    candidate.address = offset;
    candidate.type = MapType::Map2D;
    
    if (offset + 64 > m_size) {
        return candidate;
    }
    
    // Try different sizes with more granular steps
    const size_t testSizes[] = {8, 12, 16, 20, 24, 32, 40, 48, 64, 80, 96, 128};
    double bestConfidence = 0.0;
    size_t bestSize = 0;
    
    for (size_t testSize : testSizes) {
        if (offset + testSize * 2 > m_size) {
            continue;
        }
        
        auto pattern = PatternAnalyzer::analyze2D(m_data, offset, testSize, 2);
        
        // Enhanced confidence calculation
        double confidence = 0.0;
        
        // Monotonic patterns are common in ECU maps
        if (pattern.isMonotonic) {
            confidence += 0.25;
        }
        
        // Variance indicates data variation (not all zeros/ones)
        if (pattern.variance > 100 && pattern.variance < 1000000) {
            confidence += 0.25;
        } else if (pattern.variance > 0) {
            confidence += 0.1;
        }
        
        // Mean should be in reasonable range
        if (pattern.mean > 100 && pattern.mean < 60000) {
            confidence += 0.2;
        } else if (pattern.mean > 0 && pattern.mean < 65535) {
            confidence += 0.1;
        }
        
        // Check for smooth transitions (common in tuning maps)
        if (testSize > 4) {
            size_t smoothCount = 0;
            for (size_t i = 1; i < testSize && offset + i * 2 + 1 < m_size; ++i) {
                uint16_t prev = EndiannessConverter::readLittleEndian<uint16_t>(m_data + offset + (i-1) * 2);
                uint16_t curr = EndiannessConverter::readLittleEndian<uint16_t>(m_data + offset + i * 2);
                uint16_t diff = (curr > prev) ? (curr - prev) : (prev - curr);
                if (diff < pattern.mean * 0.5) { // Relatively small change
                    smoothCount++;
                }
            }
            double smoothness = static_cast<double>(smoothCount) / (testSize - 1);
            confidence += smoothness * 0.15;
        }
        
        // Prefer common map sizes
        if (testSize == 16 || testSize == 32 || testSize == 64) {
            confidence += 0.05;
        }
        
        if (confidence > bestConfidence) {
            bestConfidence = confidence;
            bestSize = testSize;
        }
    }
    
    candidate.columns = bestSize;
    candidate.rows = 1;
    candidate.confidence = std::min(bestConfidence, 1.0); // Cap at 1.0
    
    return candidate;
}

MapCandidate MapDetector::detect3DMap(size_t offset) {
    MapCandidate candidate;
    candidate.address = offset;
    candidate.type = MapType::Map3D;
    
    if (offset + 256 > m_size) {
        return candidate;
    }
    
    // Try different matrix sizes
    const size_t testRows[] = {8, 12, 16, 20};
    const size_t testCols[] = {8, 12, 16, 20};
    
    double bestConfidence = 0.0;
    size_t bestRows = 0;
    size_t bestCols = 0;
    
    for (size_t rows : testRows) {
        for (size_t cols : testCols) {
            if (offset + rows * cols * 2 > m_size) {
                continue;
            }
            
            bool hasPattern = PatternAnalyzer::detectMatrixPattern(m_data, offset, rows, cols, 2);
            
            // Analyze first row
            auto rowPattern = PatternAnalyzer::analyze2D(m_data, offset, cols, 2);
            
            double confidence = 0.0;
            if (hasPattern) {
                confidence += 0.4;
            }
            if (rowPattern.variance > 100 && rowPattern.variance < 1000000) {
                confidence += 0.3;
            }
            if (rowPattern.mean > 0 && rowPattern.mean < 65535) {
                confidence += 0.2;
            }
            if (rowPattern.isMonotonic) {
                confidence += 0.1;
            }
            
            if (confidence > bestConfidence) {
                bestConfidence = confidence;
                bestRows = rows;
                bestCols = cols;
            }
        }
    }
    
    candidate.rows = bestRows;
    candidate.columns = bestCols;
    candidate.confidence = bestConfidence;
    
    return candidate;
}

bool MapDetector::isValidMapAddress(size_t offset, const MapCandidate& candidate) {
    if (offset + candidate.rows * candidate.columns * 2 > m_size) {
        return false;
    }
    
    // Check if address is aligned (even addresses preferred for 16-bit data)
    if (offset % 2 != 0) {
        return false;
    }
    
    // Check for reasonable values (not all zeros, not all 0xFFFF)
    size_t checkCount = std::min(candidate.rows * candidate.columns, size_t(16));
    bool hasVariation = false;
    uint16_t firstValue = EndiannessConverter::readLittleEndian<uint16_t>(m_data + offset);
    
    for (size_t i = 1; i < checkCount; ++i) {
        uint16_t value = EndiannessConverter::readLittleEndian<uint16_t>(m_data + offset + i * 2);
        if (value != firstValue) {
            hasVariation = true;
            break;
        }
    }
    
    return hasVariation;
}

} // namespace WinMMM10

