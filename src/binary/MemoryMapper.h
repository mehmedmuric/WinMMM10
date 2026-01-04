#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace WinMMM10 {

class MemoryMapper {
public:
    MemoryMapper();
    ~MemoryMapper();
    
    bool open(const std::string& filepath);
    void close();
    
    bool isOpen() const { return m_mapped; }
    size_t size() const { return m_size; }
    
    const uint8_t* data() const { return m_data; }
    uint8_t* data() { return m_data; }
    
    const uint8_t* at(size_t offset) const;
    uint8_t* at(size_t offset);
    
    bool isValidOffset(size_t offset) const {
        return offset < m_size;
    }

private:
    void* m_handle{nullptr};
    void* m_mappedData{nullptr};
    uint8_t* m_data{nullptr};
    size_t m_size{0};
    bool m_mapped{false};
    
#ifdef _WIN32
    HANDLE m_fileHandle{INVALID_HANDLE_VALUE};
    HANDLE m_mapHandle{nullptr};
#else
    int m_fileDescriptor{-1};
#endif
};

} // namespace WinMMM10

