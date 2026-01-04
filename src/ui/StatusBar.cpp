#include "StatusBar.h"
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

StatusBar::StatusBar(QWidget* parent)
    : QStatusBar(parent)
{
    m_addressLabel = new QLabel("Address: 0x00000000");
    m_fileLabel = new QLabel("No file loaded");
    m_messageLabel = new QLabel("Ready");
    
    addWidget(m_addressLabel);
    addPermanentWidget(m_fileLabel);
    addPermanentWidget(m_messageLabel);
}

void StatusBar::setAddress(size_t address) {
    std::ostringstream oss;
    oss << "Address: 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << address;
    m_addressLabel->setText(QString::fromStdString(oss.str()));
}

void StatusBar::setFileInfo(const QString& filename, size_t size) {
    QString sizeStr = QString::number(size) + " bytes";
    m_fileLabel->setText(filename + " - " + sizeStr);
}

void StatusBar::setMessage(const QString& message) {
    m_messageLabel->setText(message);
}

} // namespace WinMMM10

