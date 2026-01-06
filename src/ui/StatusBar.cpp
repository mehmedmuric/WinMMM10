#include "StatusBar.h"
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

StatusBar::StatusBar(QWidget* parent)
    : QStatusBar(parent)
{
    m_addressLabel = new QLabel("Address: 0x00000000");
    m_fileLabel = new QLabel("No file loaded");
    m_selectionLabel = new QLabel("");
    m_checksumLabel = new QLabel("");
    m_messageLabel = new QLabel("Ready");
    m_safeModeLabel = new QLabel("");
    
    m_selectionLabel->setVisible(false);
    m_checksumLabel->setVisible(false);
    m_safeModeLabel->setStyleSheet("color: green; font-weight: bold;");
    
    addWidget(m_addressLabel);
    addPermanentWidget(m_selectionLabel);
    addPermanentWidget(m_checksumLabel);
    addPermanentWidget(m_safeModeLabel);
    addPermanentWidget(m_fileLabel);
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

