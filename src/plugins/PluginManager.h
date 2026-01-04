#pragma once

#include "PluginInterface.h"
#include <QPluginLoader>
#include <QObject>
#include <string>
#include <vector>
#include <memory>

namespace WinMMM10 {

struct PluginInfo {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string filepath;
    bool loaded{false};
};

class PluginManager : public QObject {
    Q_OBJECT

public:
    static PluginManager& instance();
    
    bool loadPlugin(const std::string& filepath);
    void unloadPlugin(const std::string& name);
    void unloadAll();
    
    std::vector<PluginInfo> getLoadedPlugins() const;
    PluginInterface* getPlugin(const std::string& name) const;
    
    void scanPluginDirectory(const std::string& directory);

signals:
    void pluginLoaded(const std::string& name);
    void pluginUnloaded(const std::string& name);

private:
    PluginManager(QObject* parent = nullptr);
    ~PluginManager() override;
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;
    
    struct PluginEntry {
        std::unique_ptr<QPluginLoader> loader;
        PluginInterface* interface{nullptr};
        PluginInfo info;
    };
    
    std::vector<PluginEntry> m_plugins;
};

} // namespace WinMMM10

