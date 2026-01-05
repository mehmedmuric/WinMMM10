#include "HexSearch.h"
#include "BinaryFile.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstring>

namespace WinMMM10 {

HexSearch::HexSearch(const BinaryFile* file)
    : m_binaryFile(file)
{
}

std::vector<uint8_t> HexSearch::parseHexPattern(const std::string& pattern) const {
    std::vector<uint8_t> result;
    std::string cleanPattern = pattern;
    
    // Remove spaces and common separators
    cleanPattern.erase(std::remove_if(cleanPattern.begin(), cleanPattern.end(),
        [](char c) { return c == ' ' || c == '-' || c == ':' || c == ','; }), cleanPattern.end());
    
    // Remove 0x prefix if present
    if (cleanPattern.size() >= 2 && cleanPattern.substr(0, 2) == "0x") {
        cleanPattern = cleanPattern.substr(2);
    }
    
    // Parse hex pairs
    for (size_t i = 0; i < cleanPattern.size(); i += 2) {
        if (i + 1 < cleanPattern.size()) {
            std::string byteStr = cleanPattern.substr(i, 2);
            char* end;
            unsigned long byte = std::strtoul(byteStr.c_str(), &end, 16);
            if (*end == '\0') {
                result.push_back(static_cast<uint8_t>(byte));
            }
        }
    }
    
    return result;
}

std::vector<uint8_t> HexSearch::parseTextPattern(const std::string& pattern) const {
    return std::vector<uint8_t>(pattern.begin(), pattern.end());
}

bool HexSearch::matches(const uint8_t* data, size_t dataSize, const std::vector<uint8_t>& pattern,
                       size_t offset, bool caseSensitive) const {
    if (offset + pattern.size() > dataSize) {
        return false;
    }
    
    if (caseSensitive) {
        return std::equal(pattern.begin(), pattern.end(), data + offset);
    } else {
        for (size_t i = 0; i < pattern.size(); ++i) {
            if (std::tolower(data[offset + i]) != std::tolower(pattern[i])) {
                return false;
            }
        }
        return true;
    }
}

HexSearch::SearchResult HexSearch::findNext(const std::string& pattern, size_t startAddress,
                                            SearchMode mode, bool caseSensitive) {
    SearchResult result;
    
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return result;
    }
    
    std::vector<uint8_t> searchPattern;
    if (mode == Hex) {
        searchPattern = parseHexPattern(pattern);
    } else if (mode == Text) {
        searchPattern = parseTextPattern(pattern);
    } else {
        // Pattern mode - simple wildcard support
        searchPattern = parseHexPattern(pattern);
    }
    
    if (searchPattern.empty()) {
        return result;
    }
    
    const uint8_t* data = m_binaryFile->data();
    size_t dataSize = m_binaryFile->size();
    
    for (size_t i = startAddress; i + searchPattern.size() <= dataSize; ++i) {
        if (matches(data, dataSize, searchPattern, i, caseSensitive)) {
            result.found = true;
            result.address = i;
            result.length = searchPattern.size();
            return result;
        }
    }
    
    return result;
}

HexSearch::SearchResult HexSearch::findPrevious(const std::string& pattern, size_t startAddress,
                                                SearchMode mode, bool caseSensitive) {
    SearchResult result;
    
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return result;
    }
    
    std::vector<uint8_t> searchPattern;
    if (mode == Hex) {
        searchPattern = parseHexPattern(pattern);
    } else if (mode == Text) {
        searchPattern = parseTextPattern(pattern);
    } else {
        searchPattern = parseHexPattern(pattern);
    }
    
    if (searchPattern.empty()) {
        return result;
    }
    
    const uint8_t* data = m_binaryFile->data();
    size_t dataSize = m_binaryFile->size();
    
    // Search backwards from startAddress
    size_t maxStart = (startAddress > searchPattern.size()) ? 
                      startAddress - searchPattern.size() : 0;
    
    for (size_t i = maxStart; i != SIZE_MAX; --i) {
        if (i + searchPattern.size() <= dataSize) {
            if (matches(data, dataSize, searchPattern, i, caseSensitive)) {
                result.found = true;
                result.address = i;
                result.length = searchPattern.size();
                return result;
            }
        }
        if (i == 0) break;
    }
    
    return result;
}

std::vector<HexSearch::SearchResult> HexSearch::findAll(const std::string& pattern,
                                                        SearchMode mode, bool caseSensitive) {
    std::vector<SearchResult> results;
    
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return results;
    }
    
    size_t currentAddress = 0;
    while (true) {
        SearchResult result = findNext(pattern, currentAddress, mode, caseSensitive);
        if (!result.found) {
            break;
        }
        results.push_back(result);
        currentAddress = result.address + 1;
    }
    
    return results;
}

size_t HexSearch::replace(const std::string& pattern, const std::string& replacement,
                          size_t address, SearchMode mode, bool caseSensitive) {
    SearchResult result = findNext(pattern, address, mode, caseSensitive);
    if (!result.found || result.address != address) {
        return 0;
    }
    
    // Parse replacement
    std::vector<uint8_t> replaceData;
    if (mode == Hex) {
        replaceData = parseHexPattern(replacement);
    } else {
        replaceData = parseTextPattern(replacement);
    }
    
    if (replaceData.size() != result.length) {
        return 0; // Size mismatch
    }
    
    // Write replacement (assuming BinaryFile has writeBytes)
    BinaryFile* mutableFile = const_cast<BinaryFile*>(m_binaryFile);
    mutableFile->writeBytes(result.address, replaceData);
    
    return 1;
}

size_t HexSearch::replaceAll(const std::string& pattern, const std::string& replacement,
                            SearchMode mode, bool caseSensitive) {
    auto results = findAll(pattern, mode, caseSensitive);
    if (results.empty()) {
        return 0;
    }
    
    // Parse replacement
    std::vector<uint8_t> replaceData;
    if (mode == Hex) {
        replaceData = parseHexPattern(replacement);
    } else {
        replaceData = parseTextPattern(replacement);
    }
    
    if (replaceData.size() != parseHexPattern(pattern).size() && mode == Hex) {
        return 0; // Size mismatch for hex mode
    }
    
    BinaryFile* mutableFile = const_cast<BinaryFile*>(m_binaryFile);
    size_t count = 0;
    
    // Replace in reverse order to preserve addresses
    for (auto it = results.rbegin(); it != results.rend(); ++it) {
        mutableFile->writeBytes(it->address, replaceData);
        ++count;
    }
    
    return count;
}

} // namespace WinMMM10

