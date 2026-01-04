#include "PluginManager.h"
#include <QDir>
#include <QCoreApplication>
#include <QJsonObject>
#include <QDebug>

namespace WinMMM10 {

PluginManager::PluginManager(QObject* parent)
    : QObject(parent)
{
}

PluginManager::~PluginManager() {
    unloadAll();
}

PluginManager& PluginManager::instance() {
    static PluginManager instance;
    return instance;
}

bool PluginManager::loadPlugin(const std::string& filepath) {
    auto loader = std::make_unique<QPluginLoader>(QString::fromStdString(filepath));
    
    QObject* plugin = loader->instance();
    if (!plugin) {
        qWarning() << "Failed to load plugin:" << QString::fromStdString(filepath) << loader->errorString();
        return false;
    }
    
    PluginInterface* interface = qobject_cast<PluginInterface*>(plugin);
    if (!interface) {
        qWarning() << "Plugin does not implement PluginInterface:" << QString::fromStdString(filepath);
        return false;
    }
    
    if (!interface->initialize()) {
        qWarning() << "Plugin initialization failed:" << QString::fromStdString(filepath);
        return false;
    }
    
    PluginEntry entry;
    entry.loader = std::move(loader);
    entry.interface = interface;
    entry.info.name = interface->name();
    entry.info.version = interface->version();
    entry.info.author = interface->author();
    entry.info.description = interface->description();
    entry.info.filepath = filepath;
    entry.info.loaded = true;
    
    m_plugins.push_back(std::move(entry));
    
    emit pluginLoaded(entry.info.name);
    return true;
}

void PluginManager::unloadPlugin(const std::string& name) {
    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
        if (it->info.name == name) {
            if (it->interface) {
                it->interface->shutdown();
            }
            if (it->loader) {
                it->loader->unload();
            }
            
            std::string pluginName = it->info.name;
            m_plugins.erase(it);
            emit pluginUnloaded(pluginName);
            break;
        }
    }
}

void PluginManager::unloadAll() {
    for (auto& entry : m_plugins) {
        if (entry.interface) {
            entry.interface->shutdown();
        }
        if (entry.loader) {
            entry.loader->unload();
        }
    }
    m_plugins.clear();
}

std::vector<PluginInfo> PluginManager::getLoadedPlugins() const {
    std::vector<PluginInfo> result;
    for (const auto& entry : m_plugins) {
        result.push_back(entry.info);
    }
    return result;
}

PluginInterface* PluginManager::getPlugin(const std::string& name) const {
    for (const auto& entry : m_plugins) {
        if (entry.info.name == name) {
            return entry.interface;
        }
    }
    return nullptr;
}

void PluginManager::scanPluginDirectory(const std::string& directory) {
    QDir dir(QString::fromStdString(directory));
    if (!dir.exists()) {
        return;
    }
    
    QStringList filters;
#ifdef _WIN32
    filters << "*.dll";
#else
    filters << "*.so";
#endif
    
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    for (const QFileInfo& fileInfo : files) {
        loadPlugin(fileInfo.absoluteFilePath().toStdString());
    }
}

} // namespace WinMMM10

