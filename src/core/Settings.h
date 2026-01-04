#pragma once

#include <string>

namespace WinMMM10 {

class Settings {
public:
    static Settings& instance();
    
    std::string lastProjectPath() const { return m_lastProjectPath; }
    void setLastProjectPath(const std::string& path) { m_lastProjectPath = path; }
    
    std::string lastBinaryPath() const { return m_lastBinaryPath; }
    void setLastBinaryPath(const std::string& path) { m_lastBinaryPath = path; }
    
    void load();
    void save();

private:
    Settings() = default;
    ~Settings() = default;
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    
    std::string m_lastProjectPath;
    std::string m_lastBinaryPath;
};

} // namespace WinMMM10

