#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

namespace WinMMM10 {

struct RecentFile {
    std::string filepath;
    std::string displayName;
    int64_t lastAccessed{0};
};

class ApplicationCache {
public:
    static ApplicationCache& instance();
    
    // Recent files management
    void addRecentProject(const std::string& filepath);
    void addRecentBinary(const std::string& filepath);
    std::vector<RecentFile> getRecentProjects(size_t maxCount = 20) const;
    std::vector<RecentFile> getRecentBinaries(size_t maxCount = 20) const;
    void clearRecentFiles();
    
    // Cache directory management
    std::string getCacheDirectory() const;
    std::string getTempDirectory() const;
    std::string getThumbnailDirectory() const;
    
    // Cache size management
    uint64_t getCacheSize() const;
    uint64_t getTempSize() const;
    void clearCache();
    void clearTempFiles();
    
    // Thumbnail cache
    void saveThumbnail(const std::string& key, const std::vector<uint8_t>& data);
    std::vector<uint8_t> loadThumbnail(const std::string& key) const;
    void clearThumbnails();
    
    // Load/Save cache state
    void load();
    void save();

private:
    ApplicationCache() = default;
    ~ApplicationCache() = default;
    ApplicationCache(const ApplicationCache&) = delete;
    ApplicationCache& operator=(const ApplicationCache&) = delete;
    
    void initializeDirectories();
    uint64_t calculateDirectorySize(const std::string& path) const;
    void removeDirectoryContents(const std::string& path) const;
    
    std::vector<RecentFile> m_recentProjects;
    std::vector<RecentFile> m_recentBinaries;
    std::string m_cacheDir;
    std::string m_tempDir;
    std::string m_thumbnailDir;
};

} // namespace WinMMM10

