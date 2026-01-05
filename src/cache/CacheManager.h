#pragma once

#include "ApplicationCache.h"
#include "ProjectCache.h"
#include <string>
#include <memory>
#include <cstdint>

namespace WinMMM10 {

class CacheManager {
public:
    static CacheManager& instance();
    
    // Application cache access
    ApplicationCache& applicationCache() { return ApplicationCache::instance(); }
    const ApplicationCache& applicationCache() const { return ApplicationCache::instance(); }
    
    // Project cache management
    void setCurrentProject(const std::string& projectPath);
    ProjectCache* currentProjectCache() { return m_currentProjectCache.get(); }
    const ProjectCache* currentProjectCache() const { return m_currentProjectCache.get(); }
    
    // Combined cache operations
    uint64_t getTotalCacheSize() const;
    void clearAllCaches();
    void clearApplicationCache();
    void clearProjectCache();
    void clearTempFiles();
    
    // Cache statistics
    struct CacheStats {
        uint64_t applicationCacheSize{0};
        uint64_t projectCacheSize{0};
        uint64_t tempFilesSize{0};
        uint64_t totalSize{0};
        size_t recentProjectsCount{0};
        size_t recentBinariesCount{0};
    };
    
    CacheStats getCacheStats() const;

private:
    CacheManager() = default;
    ~CacheManager() = default;
    CacheManager(const CacheManager&) = delete;
    CacheManager& operator=(const CacheManager&) = delete;
    
    std::unique_ptr<ProjectCache> m_currentProjectCache;
};

} // namespace WinMMM10

