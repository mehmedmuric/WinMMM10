#include "CacheManager.h"

namespace WinMMM10 {

CacheManager& CacheManager::instance() {
    static CacheManager instance;
    return instance;
}

void CacheManager::setCurrentProject(const std::string& projectPath) {
    if (projectPath.empty()) {
        m_currentProjectCache.reset();
    } else {
        m_currentProjectCache = std::make_unique<ProjectCache>(projectPath);
        m_currentProjectCache->load();
    }
}

uint64_t CacheManager::getTotalCacheSize() const {
    uint64_t total = 0;
    total += applicationCache().getCacheSize();
    total += applicationCache().getTempSize();
    if (m_currentProjectCache) {
        total += m_currentProjectCache->getCacheSize();
    }
    return total;
}

void CacheManager::clearAllCaches() {
    clearApplicationCache();
    clearProjectCache();
    clearTempFiles();
}

void CacheManager::clearApplicationCache() {
    applicationCache().clearCache();
    applicationCache().clearRecentFiles();
    applicationCache().clearThumbnails();
}

void CacheManager::clearProjectCache() {
    if (m_currentProjectCache) {
        m_currentProjectCache->clearAll();
    }
}

void CacheManager::clearTempFiles() {
    applicationCache().clearTempFiles();
}

CacheManager::CacheStats CacheManager::getCacheStats() const {
    CacheStats stats;
    stats.applicationCacheSize = applicationCache().getCacheSize();
    stats.tempFilesSize = applicationCache().getTempSize();
    if (m_currentProjectCache) {
        stats.projectCacheSize = m_currentProjectCache->getCacheSize();
    }
    stats.totalSize = stats.applicationCacheSize + stats.projectCacheSize + stats.tempFilesSize;
    stats.recentProjectsCount = applicationCache().getRecentProjects().size();
    stats.recentBinariesCount = applicationCache().getRecentBinaries().size();
    return stats;
}

} // namespace WinMMM10

