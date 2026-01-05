#include "ProjectCache.h"
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include <QDateTime>
#include <QStringList>
#include <cstring>
#include <fstream>
#include <sstream>

namespace WinMMM10 {

ProjectCache::ProjectCache(const std::string& projectPath)
    : m_projectPath(projectPath)
{
}

std::string ProjectCache::getCacheFilePath() const {
    QFileInfo projectInfo(QString::fromStdString(m_projectPath));
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/projects";
    QDir dir(cacheDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    QString cacheFile = cacheDir + "/" + projectInfo.baseName() + ".cache";
    return cacheFile.toStdString();
}

void ProjectCache::cacheDetectionResults(const std::string& binaryHash,
                                         const std::vector<CachedMapData>& maps) {
    DetectionCache cache;
    cache.binaryHash = binaryHash;
    cache.detectedMaps = maps;
    cache.timestamp = QDateTime::currentMSecsSinceEpoch();
    m_detectionCache[binaryHash] = cache;
    save();
}

bool ProjectCache::hasDetectionCache(const std::string& binaryHash) const {
    return m_detectionCache.find(binaryHash) != m_detectionCache.end();
}

std::vector<CachedMapData> ProjectCache::getDetectionCache(const std::string& binaryHash) const {
    auto it = m_detectionCache.find(binaryHash);
    if (it != m_detectionCache.end()) {
        return it->second.detectedMaps;
    }
    return {};
}

void ProjectCache::clearDetectionCache() {
    m_detectionCache.clear();
    save();
}

void ProjectCache::cacheMapData(const std::string& mapName, const CachedMapData& data) {
    CachedMapData cached = data;
    cached.timestamp = QDateTime::currentMSecsSinceEpoch();
    m_mapCache[mapName] = cached;
    save();
}

bool ProjectCache::hasMapCache(const std::string& mapName) const {
    return m_mapCache.find(mapName) != m_mapCache.end();
}

CachedMapData ProjectCache::getMapCache(const std::string& mapName) const {
    auto it = m_mapCache.find(mapName);
    if (it != m_mapCache.end()) {
        return it->second;
    }
    return CachedMapData();
}

void ProjectCache::clearMapCache() {
    m_mapCache.clear();
    save();
}

uint64_t ProjectCache::calculateCacheSize() const {
    uint64_t size = 0;
    for (const auto& [hash, cache] : m_detectionCache) {
        for (const auto& map : cache.detectedMaps) {
            size += map.rawData.size();
            size += map.processedData.size() * sizeof(double);
        }
    }
    for (const auto& [name, data] : m_mapCache) {
        size += data.rawData.size();
        size += data.processedData.size() * sizeof(double);
    }
    return size;
}

uint64_t ProjectCache::getCacheSize() const {
    return calculateCacheSize();
}

void ProjectCache::clearAll() {
    m_detectionCache.clear();
    m_mapCache.clear();
    save();
}

void ProjectCache::load() {
    std::string cacheFile = getCacheFilePath();
    QFileInfo fileInfo(QString::fromStdString(cacheFile));
    if (!fileInfo.exists()) {
        return;
    }
    
    QSettings settings(QString::fromStdString(cacheFile), QSettings::IniFormat);
    
    // Load detection cache
    QStringList hashKeys = settings.childGroups();
    for (const QString& hash : hashKeys) {
        settings.beginGroup(hash);
        DetectionCache cache;
        cache.binaryHash = hash.toStdString();
        cache.timestamp = settings.value("timestamp").toLongLong();
        
        int mapCount = settings.beginReadArray("maps");
        for (int i = 0; i < mapCount; ++i) {
            settings.setArrayIndex(i);
            CachedMapData mapData;
            mapData.mapName = settings.value("name").toString().toStdString();
            mapData.address = settings.value("address").toULongLong();
            QByteArray rawData = settings.value("rawData").toByteArray();
            mapData.rawData.assign(rawData.begin(), rawData.end());
            QByteArray procData = settings.value("processedData").toByteArray();
            if (procData.size() % sizeof(double) == 0) {
                mapData.processedData.resize(procData.size() / sizeof(double));
                std::memcpy(mapData.processedData.data(), procData.data(), procData.size());
            }
            mapData.timestamp = settings.value("timestamp").toLongLong();
            cache.detectedMaps.push_back(mapData);
        }
        settings.endArray();
        settings.endGroup();
        
        m_detectionCache[cache.binaryHash] = cache;
    }
    
    // Load map cache
    settings.beginGroup("MapCache");
    QStringList mapKeys = settings.childGroups();
    for (const QString& mapName : mapKeys) {
        settings.beginGroup(mapName);
        CachedMapData mapData;
        mapData.mapName = mapName.toStdString();
        mapData.address = settings.value("address").toULongLong();
        QByteArray rawData = settings.value("rawData").toByteArray();
        mapData.rawData.assign(rawData.begin(), rawData.end());
        QByteArray procData = settings.value("processedData").toByteArray();
        if (procData.size() % sizeof(double) == 0) {
            mapData.processedData.resize(procData.size() / sizeof(double));
            std::memcpy(mapData.processedData.data(), procData.data(), procData.size());
        }
        mapData.timestamp = settings.value("timestamp").toLongLong();
        m_mapCache[mapData.mapName] = mapData;
        settings.endGroup();
    }
    settings.endGroup();
}

void ProjectCache::save() {
    std::string cacheFile = getCacheFilePath();
    QSettings settings(QString::fromStdString(cacheFile), QSettings::IniFormat);
    settings.clear();
    
    // Save detection cache
    for (const auto& [hash, cache] : m_detectionCache) {
        settings.beginGroup(hash);
        settings.setValue("timestamp", cache.timestamp);
        settings.beginWriteArray("maps");
        for (size_t i = 0; i < cache.detectedMaps.size(); ++i) {
            settings.setArrayIndex(i);
            const auto& mapData = cache.detectedMaps[i];
            settings.setValue("name", QString::fromStdString(mapData.mapName));
            settings.setValue("address", static_cast<qulonglong>(mapData.address));
            QByteArray rawData(reinterpret_cast<const char*>(mapData.rawData.data()), 
                              mapData.rawData.size());
            settings.setValue("rawData", rawData);
            QByteArray procData(reinterpret_cast<const char*>(mapData.processedData.data()),
                               mapData.processedData.size() * sizeof(double));
            settings.setValue("processedData", procData);
            settings.setValue("timestamp", mapData.timestamp);
        }
        settings.endArray();
        settings.endGroup();
    }
    
    // Save map cache
    settings.beginGroup("MapCache");
    for (const auto& [name, mapData] : m_mapCache) {
        settings.beginGroup(QString::fromStdString(name));
        settings.setValue("address", static_cast<qulonglong>(mapData.address));
        QByteArray rawData(reinterpret_cast<const char*>(mapData.rawData.data()),
                          mapData.rawData.size());
        settings.setValue("rawData", rawData);
        QByteArray procData(reinterpret_cast<const char*>(mapData.processedData.data()),
                           mapData.processedData.size() * sizeof(double));
        settings.setValue("processedData", procData);
        settings.setValue("timestamp", mapData.timestamp);
        settings.endGroup();
    }
    settings.endGroup();
}

} // namespace WinMMM10

