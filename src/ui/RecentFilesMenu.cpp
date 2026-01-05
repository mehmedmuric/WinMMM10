#include "RecentFilesMenu.h"
#include <QFileInfo>

namespace WinMMM10 {

RecentFilesMenu::RecentFilesMenu(const QString& title, QWidget* parent)
    : QMenu(title, parent)
{
    m_clearAction = addAction("Clear List", this, &RecentFilesMenu::clearRecentFiles);
    m_clearAction->setEnabled(false);
    addSeparator();
}

void RecentFilesMenu::updateRecentFiles(const std::vector<std::string>& files) {
    m_files = files;
    rebuildMenu();
}

void RecentFilesMenu::clearRecentFiles() {
    m_files.clear();
    rebuildMenu();
    emit fileSelected(""); // Signal clear
}

void RecentFilesMenu::rebuildMenu() {
    // Remove old actions (except clear action and separator)
    for (QAction* action : m_actions) {
        removeAction(action);
        delete action;
    }
    m_actions.clear();
    
    // Add new actions
    size_t count = std::min(m_files.size(), MAX_RECENT_FILES);
    for (size_t i = 0; i < count; ++i) {
        QFileInfo info(QString::fromStdString(m_files[i]));
        QString displayName = QString("%1. %2").arg(i + 1).arg(info.fileName());
        
        QAction* action = addAction(displayName);
        action->setData(QString::fromStdString(m_files[i]));
        action->setToolTip(QString::fromStdString(m_files[i]));
        connect(action, &QAction::triggered, this, &RecentFilesMenu::onFileTriggered);
        m_actions.push_back(action);
    }
    
    m_clearAction->setEnabled(!m_files.empty());
}

void RecentFilesMenu::onFileTriggered() {
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QString filepath = action->data().toString();
        emit fileSelected(filepath);
    }
}

} // namespace WinMMM10

