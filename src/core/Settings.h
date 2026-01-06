#pragma once

#include <string>
#include <QByteArray>

namespace WinMMM10 {

class Settings {
public:
    static Settings& instance();
    
    std::string lastProjectPath() const { return m_lastProjectPath; }
    void setLastProjectPath(const std::string& path) { m_lastProjectPath = path; }
    
    std::string lastBinaryPath() const { return m_lastBinaryPath; }
    void setLastBinaryPath(const std::string& path) { m_lastBinaryPath = path; }
    
    bool autoCleanupCache() const { return m_autoCleanupCache; }
    void setAutoCleanupCache(bool enabled) { m_autoCleanupCache = enabled; }
    
    bool safeModeEnabled() const { return m_safeModeEnabled; }
    void setSafeModeEnabled(bool enabled) { m_safeModeEnabled = enabled; }
    
    QByteArray windowGeometry() const { return m_windowGeometry; }
    void setWindowGeometry(const QByteArray& geometry) { m_windowGeometry = geometry; }
    
    QByteArray windowState() const { return m_windowState; }
    void setWindowState(const QByteArray& state) { m_windowState = state; }
    
    void load();
    void save();

private:
    Settings() = default;
    ~Settings() = default;
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    
    std::string m_lastProjectPath;
    std::string m_lastBinaryPath;
    bool m_autoCleanupCache{false};
    bool m_safeModeEnabled{true}; // Default: enabled
    QByteArray m_windowGeometry;
    QByteArray m_windowState;
};

} // namespace WinMMM10

