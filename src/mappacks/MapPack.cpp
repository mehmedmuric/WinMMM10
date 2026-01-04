#include "MapPack.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QStandardPaths>
#include <stdexcept>

namespace WinMMM10 {

MapPack::MapPack() = default;

void MapPack::addMap(const MapDefinition& map) {
    m_maps.push_back(map);
}

void MapPack::removeMap(size_t index) {
    if (index >= m_maps.size()) {
        return;
    }
    m_maps.erase(m_maps.begin() + index);
}

MapDefinition& MapPack::getMap(size_t index) {
    if (index >= m_maps.size()) {
        throw std::out_of_range("Map index out of range");
    }
    return m_maps[index];
}

const MapDefinition& MapPack::getMap(size_t index) const {
    if (index >= m_maps.size()) {
        throw std::out_of_range("Map index out of range");
    }
    return m_maps[index];
}

bool MapPack::saveToFile(const std::string& filepath) const {
    QJsonObject root;
    
    // Save info
    QJsonObject infoObj;
    infoObj["name"] = QString::fromStdString(m_info.name);
    infoObj["version"] = QString::fromStdString(m_info.version);
    infoObj["author"] = QString::fromStdString(m_info.author);
    infoObj["description"] = QString::fromStdString(m_info.description);
    infoObj["ecuName"] = QString::fromStdString(m_info.ecuName);
    infoObj["ecuId"] = QString::fromStdString(m_info.ecuId);
    infoObj["fileHash"] = QString::fromStdString(m_info.fileHash);
    
    QJsonArray tagsArray;
    for (const auto& tag : m_info.tags) {
        tagsArray.append(QString::fromStdString(tag));
    }
    infoObj["tags"] = tagsArray;
    root["info"] = infoObj;
    
    // Save maps
    QJsonArray mapsArray;
    for (const MapDefinition& map : m_maps) {
        QJsonObject mapObj;
        mapObj["name"] = QString::fromStdString(map.name());
        mapObj["address"] = static_cast<qint64>(map.address());
        mapObj["type"] = (map.type() == MapType::Map3D) ? "3D" : "2D";
        mapObj["rows"] = static_cast<int>(map.rows());
        mapObj["columns"] = static_cast<int>(map.columns());
        mapObj["dataType"] = static_cast<int>(map.dataType());
        mapObj["factor"] = map.factor();
        mapObj["offset"] = map.offset();
        mapObj["unit"] = QString::fromStdString(map.unit());
        
        QJsonObject xAxisObj;
        const MapAxis& xAxis = map.xAxis();
        xAxisObj["address"] = static_cast<qint64>(xAxis.address());
        xAxisObj["count"] = static_cast<int>(xAxis.count());
        xAxisObj["dataType"] = static_cast<int>(xAxis.dataType());
        xAxisObj["factor"] = xAxis.factor();
        xAxisObj["offset"] = xAxis.offset();
        xAxisObj["name"] = QString::fromStdString(xAxis.name());
        xAxisObj["unit"] = QString::fromStdString(xAxis.unit());
        mapObj["xAxis"] = xAxisObj;
        
        if (map.type() == MapType::Map3D) {
            QJsonObject yAxisObj;
            const MapAxis& yAxis = map.yAxis();
            yAxisObj["address"] = static_cast<qint64>(yAxis.address());
            yAxisObj["count"] = static_cast<int>(yAxis.count());
            yAxisObj["dataType"] = static_cast<int>(yAxis.dataType());
            yAxisObj["factor"] = yAxis.factor();
            yAxisObj["offset"] = yAxis.offset();
            yAxisObj["name"] = QString::fromStdString(yAxis.name());
            yAxisObj["unit"] = QString::fromStdString(yAxis.unit());
            mapObj["yAxis"] = yAxisObj;
        }
        
        mapsArray.append(mapObj);
    }
    root["maps"] = mapsArray;
    
    QJsonDocument doc(root);
    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson());
    return true;
}

bool MapPack::loadFromFile(const std::string& filepath) {
    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }
    
    QJsonObject root = doc.object();
    
    // Load info
    QJsonObject infoObj = root["info"].toObject();
    m_info.name = infoObj["name"].toString().toStdString();
    m_info.version = infoObj["version"].toString().toStdString();
    m_info.author = infoObj["author"].toString().toStdString();
    m_info.description = infoObj["description"].toString().toStdString();
    m_info.ecuName = infoObj["ecuName"].toString().toStdString();
    m_info.ecuId = infoObj["ecuId"].toString().toStdString();
    m_info.fileHash = infoObj["fileHash"].toString().toStdString();
    
    QJsonArray tagsArray = infoObj["tags"].toArray();
    m_info.tags.clear();
    for (const QJsonValue& tagValue : tagsArray) {
        m_info.tags.push_back(tagValue.toString().toStdString());
    }
    
    // Load maps
    m_maps.clear();
    QJsonArray mapsArray = root["maps"].toArray();
    for (const QJsonValue& mapValue : mapsArray) {
        QJsonObject mapObj = mapValue.toObject();
        MapDefinition map;
        map.setName(mapObj["name"].toString().toStdString());
        map.setAddress(static_cast<size_t>(mapObj["address"].toInt()));
        map.setType(mapObj["type"].toString() == "3D" ? MapType::Map3D : MapType::Map2D);
        map.setRows(static_cast<size_t>(mapObj["rows"].toInt()));
        map.setColumns(static_cast<size_t>(mapObj["columns"].toInt()));
        map.setDataType(static_cast<uint16_t>(mapObj["dataType"].toInt()));
        map.setFactor(mapObj["factor"].toDouble());
        map.setOffset(mapObj["offset"].toDouble());
        map.setUnit(mapObj["unit"].toString().toStdString());
        
        QJsonObject xAxisObj = mapObj["xAxis"].toObject();
        MapAxis& xAxis = map.xAxis();
        xAxis.setAddress(static_cast<size_t>(xAxisObj["address"].toInt()));
        xAxis.setCount(static_cast<size_t>(xAxisObj["count"].toInt()));
        xAxis.setDataType(static_cast<uint16_t>(xAxisObj["dataType"].toInt()));
        xAxis.setFactor(xAxisObj["factor"].toDouble());
        xAxis.setOffset(xAxisObj["offset"].toDouble());
        xAxis.setName(xAxisObj["name"].toString().toStdString());
        xAxis.setUnit(xAxisObj["unit"].toString().toStdString());
        
        if (map.type() == MapType::Map3D) {
            QJsonObject yAxisObj = mapObj["yAxis"].toObject();
            MapAxis& yAxis = map.yAxis();
            yAxis.setAddress(static_cast<size_t>(yAxisObj["address"].toInt()));
            yAxis.setCount(static_cast<size_t>(yAxisObj["count"].toInt()));
            yAxis.setDataType(static_cast<uint16_t>(yAxisObj["dataType"].toInt()));
            yAxis.setFactor(yAxisObj["factor"].toDouble());
            yAxis.setOffset(yAxisObj["offset"].toDouble());
            yAxis.setName(yAxisObj["name"].toString().toStdString());
            yAxis.setUnit(yAxisObj["unit"].toString().toStdString());
        }
        
        m_maps.push_back(map);
    }
    
    return true;
}

MapPackManager& MapPackManager::instance() {
    static MapPackManager instance;
    return instance;
}

bool MapPackManager::loadMapPack(const std::string& filepath) {
    MapPack pack;
    if (pack.loadFromFile(filepath)) {
        m_installedPacks.push_back(pack);
        return true;
    }
    return false;
}

bool MapPackManager::saveMapPack(const MapPack& pack, const std::string& filepath) {
    return pack.saveToFile(filepath);
}

std::vector<MapPackInfo> MapPackManager::getInstalledMapPacks() const {
    std::vector<MapPackInfo> result;
    for (const auto& pack : m_installedPacks) {
        result.push_back(pack.info());
    }
    return result;
}

} // namespace WinMMM10

