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
}

void Settings::save() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "WinMMM10", "Editor");
    settings.setValue("lastProjectPath", QString::fromStdString(m_lastProjectPath));
    settings.setValue("lastBinaryPath", QString::fromStdString(m_lastBinaryPath));
}

} // namespace WinMMM10

