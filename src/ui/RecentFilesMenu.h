#pragma once

#include <QMenu>
#include <QAction>
#include <vector>
#include <string>

namespace WinMMM10 {

class RecentFilesMenu : public QMenu {
    Q_OBJECT

public:
    explicit RecentFilesMenu(const QString& title, QWidget* parent = nullptr);
    ~RecentFilesMenu() override = default;
    
    void updateRecentFiles(const std::vector<std::string>& files);
    void clearRecentFiles();

signals:
    void fileSelected(const QString& filepath);

private slots:
    void onFileTriggered();

private:
    void rebuildMenu();
    
    std::vector<std::string> m_files;
    std::vector<QAction*> m_actions;
    QAction* m_clearAction{nullptr};
    static constexpr size_t MAX_RECENT_FILES = 20;
};

} // namespace WinMMM10

