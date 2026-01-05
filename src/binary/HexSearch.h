#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

namespace WinMMM10 {

class BinaryFile;

class HexSearch {
public:
    HexSearch(const BinaryFile* file);
    ~HexSearch() = default;
    
    enum SearchMode {
        Hex,
        Text,
        Pattern
    };
    
    struct SearchResult {
        size_t address{0};
        size_t length{0};
        bool found{false};
    };
    
    SearchResult findNext(const std::string& pattern, size_t startAddress = 0, 
                        SearchMode mode = Hex, bool caseSensitive = false);
    SearchResult findPrevious(const std::string& pattern, size_t startAddress = 0,
                             SearchMode mode = Hex, bool caseSensitive = false);
    
    std::vector<SearchResult> findAll(const std::string& pattern, SearchMode mode = Hex,
                                     bool caseSensitive = false);
    
    size_t replace(const std::string& pattern, const std::string& replacement,
                  size_t address, SearchMode mode = Hex, bool caseSensitive = false);
    size_t replaceAll(const std::string& pattern, const std::string& replacement,
                     SearchMode mode = Hex, bool caseSensitive = false);

private:
    std::vector<uint8_t> parseHexPattern(const std::string& pattern) const;
    std::vector<uint8_t> parseTextPattern(const std::string& pattern) const;
    bool matches(const uint8_t* data, size_t dataSize, const std::vector<uint8_t>& pattern,
                size_t offset, bool caseSensitive) const;
    
    const BinaryFile* m_binaryFile;
};

} // namespace WinMMM10

