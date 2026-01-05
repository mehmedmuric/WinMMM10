#include "CacheSettingsDialog.h"
#include "../cache/CacheManager.h"
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QDialogButtonBox>

namespace WinMMM10 {

CacheSettingsDialog::CacheSettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Cache Settings");
    setModal(true);
    resize(500, 400);
    
    setupUI();
    updateCacheInfo();
}

void CacheSettingsDialog::setupUI() {
    auto* layout = new QVBoxLayout(this);
    
    // Cache information group
    auto* infoGroup = new QGroupBox("Cache Information", this);
    auto* infoLayout = new QFormLayout(infoGroup);
    
    m_appCacheSizeLabel = new QLabel("0 B", this);
    m_projectCacheSizeLabel = new QLabel("0 B", this);
    m_tempFilesSizeLabel = new QLabel("0 B", this);
    m_totalSizeLabel = new QLabel("0 B", this);
    m_recentProjectsLabel = new QLabel("0", this);
    m_recentBinariesLabel = new QLabel("0", this);
    
    infoLayout->addRow("Application Cache:", m_appCacheSizeLabel);
    infoLayout->addRow("Project Cache:", m_projectCacheSizeLabel);
    infoLayout->addRow("Temporary Files:", m_tempFilesSizeLabel);
    infoLayout->addRow("Total Cache Size:", m_totalSizeLabel);
    infoLayout->addRow("Recent Projects:", m_recentProjectsLabel);
    infoLayout->addRow("Recent Binaries:", m_recentBinariesLabel);
    
    layout->addWidget(infoGroup);
    
    // Cache actions group
    auto* actionsGroup = new QGroupBox("Cache Actions", this);
    auto* actionsLayout = new QVBoxLayout(actionsGroup);
    
    m_clearAppCacheButton = new QPushButton("Clear Application Cache", this);
    m_clearProjectCacheButton = new QPushButton("Clear Project Cache", this);
    m_clearTempButton = new QPushButton("Clear Temporary Files", this);
    m_clearAllButton = new QPushButton("Clear All Caches", this);
    m_refreshButton = new QPushButton("Refresh", this);
    
    connect(m_clearAppCacheButton, &QPushButton::clicked, this, &CacheSettingsDialog::onClearApplicationCache);
    connect(m_clearProjectCacheButton, &QPushButton::clicked, this, &CacheSettingsDialog::onClearProjectCache);
    connect(m_clearTempButton, &QPushButton::clicked, this, &CacheSettingsDialog::onClearTempFiles);
    connect(m_clearAllButton, &QPushButton::clicked, this, &CacheSettingsDialog::onClearAll);
    connect(m_refreshButton, &QPushButton::clicked, this, &CacheSettingsDialog::updateCacheInfo);
    
    actionsLayout->addWidget(m_clearAppCacheButton);
    actionsLayout->addWidget(m_clearProjectCacheButton);
    actionsLayout->addWidget(m_clearTempButton);
    actionsLayout->addWidget(m_clearAllButton);
    actionsLayout->addWidget(m_refreshButton);
    
    layout->addWidget(actionsGroup);
    
    // Options
    m_autoCleanupCheckBox = new QCheckBox("Auto-cleanup on exit", this);
    layout->addWidget(m_autoCleanupCheckBox);
    
    // Dialog buttons
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

void CacheSettingsDialog::formatSize(QLabel* label, uint64_t bytes) {
    QString sizeStr;
    if (bytes < 1024) {
        sizeStr = QString("%1 B").arg(bytes);
    } else if (bytes < 1024 * 1024) {
        sizeStr = QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 2);
    } else if (bytes < 1024ULL * 1024 * 1024) {
        sizeStr = QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 2);
    } else {
        sizeStr = QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }
    label->setText(sizeStr);
}

void CacheSettingsDialog::updateCacheInfo() {
    auto& cacheManager = CacheManager::instance();
    auto stats = cacheManager.getCacheStats();
    
    formatSize(m_appCacheSizeLabel, stats.applicationCacheSize);
    formatSize(m_projectCacheSizeLabel, stats.projectCacheSize);
    formatSize(m_tempFilesSizeLabel, stats.tempFilesSize);
    formatSize(m_totalSizeLabel, stats.totalSize);
    
    m_recentProjectsLabel->setText(QString::number(stats.recentProjectsCount));
    m_recentBinariesLabel->setText(QString::number(stats.recentBinariesCount));
}

void CacheSettingsDialog::onClearApplicationCache() {
    int ret = QMessageBox::question(this, "Clear Application Cache",
                                   "Are you sure you want to clear the application cache?\n"
                                   "This will remove recent files list and thumbnails.",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        CacheManager::instance().clearApplicationCache();
        updateCacheInfo();
        QMessageBox::information(this, "Cache Cleared", "Application cache has been cleared.");
    }
}

void CacheSettingsDialog::onClearProjectCache() {
    int ret = QMessageBox::question(this, "Clear Project Cache",
                                   "Are you sure you want to clear the project cache?\n"
                                   "This will remove cached map detection results.",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        CacheManager::instance().clearProjectCache();
        updateCacheInfo();
        QMessageBox::information(this, "Cache Cleared", "Project cache has been cleared.");
    }
}

void CacheSettingsDialog::onClearTempFiles() {
    int ret = QMessageBox::question(this, "Clear Temporary Files",
                                   "Are you sure you want to clear temporary files?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        CacheManager::instance().clearTempFiles();
        updateCacheInfo();
        QMessageBox::information(this, "Cache Cleared", "Temporary files have been cleared.");
    }
}

void CacheSettingsDialog::onClearAll() {
    int ret = QMessageBox::question(this, "Clear All Caches",
                                   "Are you sure you want to clear ALL caches?\n"
                                   "This will remove all cached data including recent files.",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        CacheManager::instance().clearAllCaches();
        updateCacheInfo();
        QMessageBox::information(this, "Cache Cleared", "All caches have been cleared.");
    }
}

} // namespace WinMMM10

