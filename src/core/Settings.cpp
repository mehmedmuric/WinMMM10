#include "Settings.h"
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <stdexcept>

namespace WinMMM10 {

Settings& Settings::instance() {
    static Settings instance;
    return instance;
}

void Settings::load() {
    try {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "WinMMM10", "Editor");
        
        // Check if settings object is valid
        if (settings.status() != QSettings::NoError) {
            qWarning() << "Settings: QSettings error, using defaults";
            return;
        }
        
        m_lastProjectPath = settings.value("lastProjectPath", "").toString().toStdString();
        m_lastBinaryPath = settings.value("lastBinaryPath", "").toString().toStdString();
        m_autoCleanupCache = settings.value("autoCleanupCache", false).toBool();
    }
    catch (const std::exception& e) {
        qWarning() << "Settings: Exception during load:" << e.what();
        // Use defaults - settings are optional
    }
    catch (...) {
        qWarning() << "Settings: Unknown exception during load";
        // Use defaults - settings are optional
    }
}

void Settings::save() {
    try {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "WinMMM10", "Editor");
        
        // Check if settings object is valid
        if (settings.status() != QSettings::NoError) {
            qWarning() << "Settings: QSettings error during save";
            return;
        }
        
        settings.setValue("lastProjectPath", QString::fromStdString(m_lastProjectPath));
        settings.setValue("lastBinaryPath", QString::fromStdString(m_lastBinaryPath));
        settings.setValue("autoCleanupCache", m_autoCleanupCache);
        settings.sync();
    }
    catch (const std::exception& e) {
        qWarning() << "Settings: Exception during save:" << e.what();
    }
    catch (...) {
        qWarning() << "Settings: Unknown exception during save";
    }
}

} // namespace WinMMM10

