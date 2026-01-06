#include "HexEditorWidget.h"
#include <QMessageBox>
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

HexEditorWidget::HexEditorWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);
    
    // Toolbar with better spacing
    auto* toolbar = new QHBoxLayout();
    toolbar->setSpacing(6);
    toolbar->setContentsMargins(0, 0, 0, 0);
    
    QLabel* addressLabel = new QLabel("Address:");
    addressLabel->setMinimumWidth(60);
    toolbar->addWidget(addressLabel);
    
    m_addressEdit = new QLineEdit();
    m_addressEdit->setPlaceholderText("0x00000000");
    m_addressEdit->setMinimumWidth(120);
    m_addressEdit->setMaximumWidth(180);
    connect(m_addressEdit, &QLineEdit::returnPressed, this, &HexEditorWidget::onGoToClicked);
    toolbar->addWidget(m_addressEdit);
    
    auto* goButton = new QPushButton("Go To");
    goButton->setMinimumWidth(60);
    connect(goButton, &QPushButton::clicked, this, &HexEditorWidget::onGoToClicked);
    toolbar->addWidget(goButton);
    
    toolbar->addStretch();
    
    m_statusLabel = new QLabel("Ready");
    m_statusLabel->setMinimumWidth(100);
    toolbar->addWidget(m_statusLabel);
    
    layout->addLayout(toolbar);
    
    // Hex editor - takes remaining space
    m_hexEditor = new HexEditor(this);
    m_hexEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_hexEditor, &HexEditor::addressChanged, this, &HexEditorWidget::onAddressChanged);
    layout->addWidget(m_hexEditor, 1); // Stretch factor 1 for responsive sizing
    
    setLayout(layout);
}

void HexEditorWidget::setBinaryFile(BinaryFile* file) {
    m_hexEditor->setBinaryFile(file);
}

void HexEditorWidget::onGoToClicked() {
    QString text = m_addressEdit->text();
    bool ok;
    size_t address = 0;
    
    if (text.startsWith("0x") || text.startsWith("0X")) {
        address = text.toULongLong(&ok, 16);
    } else {
        address = text.toULongLong(&ok, 16);
        if (!ok) {
            address = text.toULongLong(&ok, 10);
        }
    }
    
    if (ok && m_hexEditor->binaryFile() && address < m_hexEditor->binaryFile()->size()) {
        m_hexEditor->goToAddress(address);
    } else {
        QMessageBox::warning(this, "Invalid Address", "Please enter a valid address.");
    }
}

void HexEditorWidget::onAddressChanged(size_t address) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << address;
    m_addressEdit->setText(QString::fromStdString(oss.str()));
    
    if (m_hexEditor->binaryFile()) {
        uint8_t byte = m_hexEditor->binaryFile()->readByte(address);
        std::ostringstream status;
        status << "Address: 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << address
               << " Value: 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(byte)
               << " (" << static_cast<int>(byte) << ")";
        m_statusLabel->setText(QString::fromStdString(status.str()));
    }
}

} // namespace WinMMM10

