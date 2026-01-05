#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QGroupBox>
#include <QCheckBox>

namespace WinMMM10 {

class CacheSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit CacheSettingsDialog(QWidget* parent = nullptr);
    ~CacheSettingsDialog() override = default;

private slots:
    void updateCacheInfo();
    void onClearApplicationCache();
    void onClearProjectCache();
    void onClearTempFiles();
    void onClearAll();
    void formatSize(QLabel* label, uint64_t bytes);

private:
    void setupUI();
    
    QLabel* m_appCacheSizeLabel{nullptr};
    QLabel* m_projectCacheSizeLabel{nullptr};
    QLabel* m_tempFilesSizeLabel{nullptr};
    QLabel* m_totalSizeLabel{nullptr};
    QLabel* m_recentProjectsLabel{nullptr};
    QLabel* m_recentBinariesLabel{nullptr};
    
    QPushButton* m_clearAppCacheButton{nullptr};
    QPushButton* m_clearProjectCacheButton{nullptr};
    QPushButton* m_clearTempButton{nullptr};
    QPushButton* m_clearAllButton{nullptr};
    QPushButton* m_refreshButton{nullptr};
    
    QCheckBox* m_autoCleanupCheckBox{nullptr};
};

} // namespace WinMMM10

