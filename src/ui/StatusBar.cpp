#include "StatusBar.h"
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

StatusBar::StatusBar(QWidget* parent)
    : QStatusBar(parent)
{
    // Set minimum height for better visibility
    setMinimumHeight(24);
    
    // Address label (left side, expandable)
    m_addressLabel = new QLabel("Address: 0x00000000");
    m_addressLabel->setMinimumWidth(150);
    addWidget(m_addressLabel);
    
    // Add stretch to push permanent widgets to the right
    addPermanentWidget(new QLabel("")); // Spacer
    
    // Selection info (right side, permanent)
    m_selectionLabel = new QLabel("");
    m_selectionLabel->setVisible(false);
    m_selectionLabel->setMinimumWidth(200);
    addPermanentWidget(m_selectionLabel);
    
    // Checksum status (right side, permanent)
    m_checksumLabel = new QLabel("");
    m_checksumLabel->setVisible(false);
    m_checksumLabel->setMinimumWidth(150);
    addPermanentWidget(m_checksumLabel);
    
    // Safe Mode indicator (right side, permanent)
    m_safeModeLabel = new QLabel("");
    m_safeModeLabel->setStyleSheet("color: green; font-weight: bold; padding: 0 8px;");
    m_safeModeLabel->setMinimumWidth(120);
    addPermanentWidget(m_safeModeLabel);
    
    // File info (right side, permanent)
    m_fileLabel = new QLabel("No file loaded");
    m_fileLabel->setMinimumWidth(200);
    m_fileLabel->setTextFormat(Qt::PlainText);
    m_fileLabel->setTextInteractionFlags(Qt::NoTextInteraction);
    addPermanentWidget(m_fileLabel);
    
    // Status message (right side, permanent)
    m_messageLabel = new QLabel("Ready");
    m_messageLabel->setMinimumWidth(150);
    addPermanentWidget(m_messageLabel);
}

void StatusBar::setAddress(size_t address) {
    std::ostringstream oss;
    oss << "Address: 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << address;
    m_addressLabel->setText(QString::fromStdString(oss.str()));
}

void StatusBar::formatSize(QLabel* label, size_t bytes) {
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

void StatusBar::setFileInfo(const QString& filename, size_t size) {
    QString sizeStr;
    formatSize(m_fileLabel, size);
    m_fileLabel->setText(filename + " - " + m_fileLabel->text());
}

void StatusBar::setMessage(const QString& message) {
    m_messageLabel->setText(message);
}

void StatusBar::setSelectionInfo(size_t startAddress, size_t endAddress, size_t size) {
    std::ostringstream oss;
    oss << "Selection: 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << startAddress;
    if (endAddress != startAddress) {
        oss << " - 0x" << std::setw(8) << endAddress;
    }
    oss << " (" << std::dec << size << " bytes)";
    m_selectionLabel->setText(QString::fromStdString(oss.str()));
    m_selectionLabel->setVisible(true);
}

void StatusBar::clearSelectionInfo() {
    m_selectionLabel->setText("");
    m_selectionLabel->setVisible(false);
}

void StatusBar::setChecksumStatus(const QString& status) {
    m_checksumLabel->setText("Checksum: " + status);
    m_checksumLabel->setVisible(true);
}

void StatusBar::clearChecksumStatus() {
    m_checksumLabel->setText("");
    m_checksumLabel->setVisible(false);
}

void StatusBar::setSafeModeStatus(bool enabled) {
    if (enabled) {
        m_safeModeLabel->setText("SAFE MODE ON");
        m_safeModeLabel->setStyleSheet("color: green; font-weight: bold;");
        m_safeModeLabel->setVisible(true);
    } else {
        m_safeModeLabel->setText("");
        m_safeModeLabel->setVisible(false);
    }
}

} // namespace WinMMM10

