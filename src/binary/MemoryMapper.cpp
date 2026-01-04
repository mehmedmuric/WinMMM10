#include "MemoryMapper.h"
#include <stdexcept>

namespace WinMMM10 {

MemoryMapper::MemoryMapper() = default;

MemoryMapper::~MemoryMapper() {
    close();
}

bool MemoryMapper::open(const std::string& filepath) {
    close();
    
#ifdef _WIN32
    m_fileHandle = CreateFileA(
        filepath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    
    if (m_fileHandle == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(m_fileHandle, &fileSize)) {
        CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
        return false;
    }
    
    m_size = static_cast<size_t>(fileSize.QuadPart);
    
    if (m_size == 0) {
        CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
        return false;
    }
    
    m_mapHandle = CreateFileMapping(
        m_fileHandle,
        nullptr,
        PAGE_READWRITE,
        0,
        0,
        nullptr
    );
    
    if (!m_mapHandle) {
        CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
        return false;
    }
    
    m_mappedData = MapViewOfFile(
        m_mapHandle,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        m_size
    );
    
    if (!m_mappedData) {
        CloseHandle(m_mapHandle);
        CloseHandle(m_fileHandle);
        m_mapHandle = nullptr;
        m_fileHandle = INVALID_HANDLE_VALUE;
        return false;
    }
    
    m_data = static_cast<uint8_t*>(m_mappedData);
    m_mapped = true;
    return true;
#else
    m_fileDescriptor = ::open(filepath.c_str(), O_RDWR);
    if (m_fileDescriptor < 0) {
        return false;
    }
    
    struct stat st;
    if (fstat(m_fileDescriptor, &st) < 0) {
        ::close(m_fileDescriptor);
        m_fileDescriptor = -1;
        return false;
    }
    
    m_size = static_cast<size_t>(st.st_size);
    if (m_size == 0) {
        ::close(m_fileDescriptor);
        m_fileDescriptor = -1;
        return false;
    }
    
    m_mappedData = mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fileDescriptor, 0);
    if (m_mappedData == MAP_FAILED) {
        ::close(m_fileDescriptor);
        m_fileDescriptor = -1;
        return false;
    }
    
    m_data = static_cast<uint8_t*>(m_mappedData);
    m_mapped = true;
    return true;
#endif
}

void MemoryMapper::close() {
    if (!m_mapped) {
        return;
    }
    
#ifdef _WIN32
    if (m_mappedData) {
        UnmapViewOfFile(m_mappedData);
        m_mappedData = nullptr;
    }
    if (m_mapHandle) {
        CloseHandle(m_mapHandle);
        m_mapHandle = nullptr;
    }
    if (m_fileHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
    }
#else
    if (m_mappedData) {
        munmap(m_mappedData, m_size);
        m_mappedData = nullptr;
    }
    if (m_fileDescriptor >= 0) {
        ::close(m_fileDescriptor);
        m_fileDescriptor = -1;
    }
#endif
    
    m_data = nullptr;
    m_size = 0;
    m_mapped = false;
}

const uint8_t* MemoryMapper::at(size_t offset) const {
    if (!isValidOffset(offset)) {
        return nullptr;
    }
    return m_data + offset;
}

uint8_t* MemoryMapper::at(size_t offset) {
    if (!isValidOffset(offset)) {
        return nullptr;
    }
    return m_data + offset;
}

} // namespace WinMMM10

