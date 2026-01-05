#include "ApplicationCache.h"
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QSettings>
#include <QFile>
#include <algorithm>
#include <fstream>
#include <sstream>

namespace WinMMM10 {

ApplicationCache& ApplicationCache::instance() {
    static ApplicationCache instance;
    return instance;
}

void ApplicationCache::initializeDirectories() {
    QString cacheBase = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir cacheDir(cacheBase);
    if (!cacheDir.exists()) {
        cacheDir.mkpath(".");
    }
    
    m_cacheDir = cacheBase.toStdString();
    m_tempDir = (cacheBase + "/temp").toStdString();
    m_thumbnailDir = (cacheBase + "/thumbnails").toStdString();
    
    QDir tempDir(QString::fromStdString(m_tempDir));
    if (!tempDir.exists()) {
        tempDir.mkpath(".");
    }
    
    QDir thumbDir(QString::fromStdString(m_thumbnailDir));
    if (!thumbDir.exists()) {
        thumbDir.mkpath(".");
    }
}

void ApplicationCache::addRecentProject(const std::string& filepath) {
    if (filepath.empty()) return;
    
    // Remove if already exists
    m_recentProjects.erase(
        std::remove_if(m_recentProjects.begin(), m_recentProjects.end(),
            [&](const RecentFile& f) { return f.filepath == filepath; }),
        m_recentProjects.end()
    );
    
    // Add to front
    RecentFile rf;
    rf.filepath = filepath;
    QFileInfo info(QString::fromStdString(filepath));
    rf.displayName = info.fileName().toStdString();
    rf.lastAccessed = QDateTime::currentMSecsSinceEpoch();
    m_recentProjects.insert(m_recentProjects.begin(), rf);
    
    // Limit to 20
    if (m_recentProjects.size() > 20) {
        m_recentProjects.resize(20);
    }
    
    save();
}

void ApplicationCache::addRecentBinary(const std::string& filepath) {
    if (filepath.empty()) return;
    
    // Remove if already exists
    m_recentBinaries.erase(
        std::remove_if(m_recentBinaries.begin(), m_recentBinaries.end(),
            [&](const RecentFile& f) { return f.filepath == filepath; }),
        m_recentBinaries.end()
    );
    
    // Add to front
    RecentFile rf;
    rf.filepath = filepath;
    QFileInfo info(QString::fromStdString(filepath));
    rf.displayName = info.fileName().toStdString();
    rf.lastAccessed = QDateTime::currentMSecsSinceEpoch();
    m_recentBinaries.insert(m_recentBinaries.begin(), rf);
    
    // Limit to 20
    if (m_recentBinaries.size() > 20) {
        m_recentBinaries.resize(20);
    }
    
    save();
}

std::vector<RecentFile> ApplicationCache::getRecentProjects(size_t maxCount) const {
    std::vector<RecentFile> result;
    size_t count = std::min(maxCount, m_recentProjects.size());
    result.assign(m_recentProjects.begin(), m_recentProjects.begin() + count);
    return result;
}

std::vector<RecentFile> ApplicationCache::getRecentBinaries(size_t maxCount) const {
    std::vector<RecentFile> result;
    size_t count = std::min(maxCount, m_recentBinaries.size());
    result.assign(m_recentBinaries.begin(), m_recentBinaries.begin() + count);
    return result;
}

void ApplicationCache::clearRecentFiles() {
    m_recentProjects.clear();
    m_recentBinaries.clear();
    save();
}

std::string ApplicationCache::getCacheDirectory() const {
    if (m_cacheDir.empty()) {
        const_cast<ApplicationCache*>(this)->initializeDirectories();
    }
    return m_cacheDir;
}

std::string ApplicationCache::getTempDirectory() const {
    if (m_tempDir.empty()) {
        const_cast<ApplicationCache*>(this)->initializeDirectories();
    }
    return m_tempDir;
}

std::string ApplicationCache::getThumbnailDirectory() const {
    if (m_thumbnailDir.empty()) {
        const_cast<ApplicationCache*>(this)->initializeDirectories();
    }
    return m_thumbnailDir;
}

uint64_t ApplicationCache::calculateDirectorySize(const std::string& path) const {
    uint64_t totalSize = 0;
    QDir dir(QString::fromStdString(path));
    if (!dir.exists()) return 0;
    
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& info : entries) {
        if (info.isFile()) {
            totalSize += info.size();
        } else if (info.isDir()) {
            totalSize += calculateDirectorySize(info.absoluteFilePath().toStdString());
        }
    }
    return totalSize;
}

uint64_t ApplicationCache::getCacheSize() const {
    return calculateDirectorySize(getCacheDirectory());
}

uint64_t ApplicationCache::getTempSize() const {
    return calculateDirectorySize(getTempDirectory());
}

void ApplicationCache::removeDirectoryContents(const std::string& path) const {
    QDir dir(QString::fromStdString(path));
    if (!dir.exists()) return;
    
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList entries = dir.entryInfoList();
    
    for (const QFileInfo& info : entries) {
        if (info.isFile()) {
            QFile::remove(info.absoluteFilePath());
        } else if (info.isDir()) {
            removeDirectoryContents(info.absoluteFilePath().toStdString());
            dir.rmdir(info.fileName());
        }
    }
}

void ApplicationCache::clearCache() {
    removeDirectoryContents(getCacheDirectory());
    // Recreate directories
    initializeDirectories();
}

void ApplicationCache::clearTempFiles() {
    removeDirectoryContents(getTempDirectory());
    // Recreate temp directory
    QDir tempDir(QString::fromStdString(m_tempDir));
    if (!tempDir.exists()) {
        tempDir.mkpath(".");
    }
}

void ApplicationCache::saveThumbnail(const std::string& key, const std::vector<uint8_t>& data) {
    std::string filename = getThumbnailDirectory() + "/" + key + ".thumb";
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
}

std::vector<uint8_t> ApplicationCache::loadThumbnail(const std::string& key) const {
    std::string filename = getThumbnailDirectory() + "/" + key + ".thumb";
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    return data;
}

void ApplicationCache::clearThumbnails() {
    removeDirectoryContents(getThumbnailDirectory());
    QDir thumbDir(QString::fromStdString(m_thumbnailDir));
    if (!thumbDir.exists()) {
        thumbDir.mkpath(".");
    }
}

void ApplicationCache::load() {
    initializeDirectories();
    
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "WinMMM10", "Editor");
    
    // Load recent projects
    int projectCount = settings.beginReadArray("recentProjects");
    m_recentProjects.clear();
    for (int i = 0; i < projectCount; ++i) {
        settings.setArrayIndex(i);
        RecentFile rf;
        rf.filepath = settings.value("filepath").toString().toStdString();
        rf.displayName = settings.value("displayName").toString().toStdString();
        rf.lastAccessed = settings.value("lastAccessed").toLongLong();
        m_recentProjects.push_back(rf);
    }
    settings.endArray();
    
    // Load recent binaries
    int binaryCount = settings.beginReadArray("recentBinaries");
    m_recentBinaries.clear();
    for (int i = 0; i < binaryCount; ++i) {
        settings.setArrayIndex(i);
        RecentFile rf;
        rf.filepath = settings.value("filepath").toString().toStdString();
        rf.displayName = settings.value("displayName").toString().toStdString();
        rf.lastAccessed = settings.value("lastAccessed").toLongLong();
        m_recentBinaries.push_back(rf);
    }
    settings.endArray();
}

void ApplicationCache::save() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "WinMMM10", "Editor");
    
    // Save recent projects
    settings.beginWriteArray("recentProjects");
    for (size_t i = 0; i < m_recentProjects.size(); ++i) {
        settings.setArrayIndex(static_cast<int>(i));
        settings.setValue("filepath", QString::fromStdString(m_recentProjects[i].filepath));
        settings.setValue("displayName", QString::fromStdString(m_recentProjects[i].displayName));
        settings.setValue("lastAccessed", m_recentProjects[i].lastAccessed);
    }
    settings.endArray();
    
    // Save recent binaries
    settings.beginWriteArray("recentBinaries");
    for (size_t i = 0; i < m_recentBinaries.size(); ++i) {
        settings.setArrayIndex(static_cast<int>(i));
        settings.setValue("filepath", QString::fromStdString(m_recentBinaries[i].filepath));
        settings.setValue("displayName", QString::fromStdString(m_recentBinaries[i].displayName));
        settings.setValue("lastAccessed", m_recentBinaries[i].lastAccessed);
    }
    settings.endArray();
}

} // namespace WinMMM10

