#include "ProjectManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <fstream>

namespace WinMMM10 {

ProjectManager::ProjectManager() = default;

bool ProjectManager::createProject(const std::string& filepath, const std::string& name) {
    auto project = std::make_unique<Project>();
    project->setName(name);
    project->setFilepath(filepath);
    
    m_currentProject = std::move(project);
    m_hasUnsavedChanges = true;
    return true;
}

bool ProjectManager::loadProject(const std::string& filepath) {
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
    auto project = std::make_unique<Project>();
    
    project->setName(root["name"].toString().toStdString());
    project->setFilepath(filepath);
    project->setBinaryFilepath(root["binaryFilepath"].toString().toStdString());
    project->setEcuName(root["ecuName"].toString().toStdString());
    project->setDescription(root["description"].toString().toStdString());
    
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
        
        project->addMap(map);
    }
    
    m_currentProject = std::move(project);
    m_hasUnsavedChanges = false;
    return true;
}

bool ProjectManager::saveProject() {
    if (!m_currentProject) {
        return false;
    }
    return saveProjectAs(m_currentProject->filepath());
}

bool ProjectManager::saveProjectAs(const std::string& filepath) {
    if (!m_currentProject) {
        return false;
    }
    
    QJsonObject root;
    root["name"] = QString::fromStdString(m_currentProject->name());
    root["binaryFilepath"] = QString::fromStdString(m_currentProject->binaryFilepath());
    root["ecuName"] = QString::fromStdString(m_currentProject->ecuName());
    root["description"] = QString::fromStdString(m_currentProject->description());
    
    QJsonArray mapsArray;
    for (const MapDefinition& map : m_currentProject->maps()) {
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
    m_currentProject->setFilepath(filepath);
    m_hasUnsavedChanges = false;
    return true;
}

void ProjectManager::closeProject() {
    m_currentProject.reset();
    m_hasUnsavedChanges = false;
}

} // namespace WinMMM10

