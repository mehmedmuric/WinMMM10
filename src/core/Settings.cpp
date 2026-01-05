#include "Settings.h"
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

namespace WinMMM10 {

Settings& Settings::instance() {
    static Settings instance;
    return instance;
}

void Settings::load() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "WinMMM10", "Editor");
    m_lastProjectPath = settings.value("lastProjectPath", "").toString().toStdString();
    m_lastBinaryPath = settings.value("lastBinaryPath", "").toString().toStdString();
    m_autoCleanupCache = settings.value("autoCleanupCache", false).toBool();
}

void Settings::save() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "WinMMM10", "Editor");
    settings.setValue("lastProjectPath", QString::fromStdString(m_lastProjectPath));
    settings.setValue("lastBinaryPath", QString::fromStdString(m_lastBinaryPath));
    settings.setValue("autoCleanupCache", m_autoCleanupCache);
}

} // namespace WinMMM10

