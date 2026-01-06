#include "HexEditor.h"
#include <QPainter>
#include <QScrollBar>
#include <QWheelEvent>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

HexEditor::HexEditor(QWidget* parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    
    m_font = QFont("Consolas", 10);
    QFontMetrics fm(m_font);
    m_charWidth = fm.horizontalAdvance('0');
    m_lineHeight = fm.height() + 4;
    
    m_scrollBar = new QScrollBar(Qt::Vertical, this);
    connect(m_scrollBar, &QScrollBar::valueChanged, this, [this](int value) {
        m_firstVisibleAddress = static_cast<size_t>(value) * m_bytesPerLine;
        update();
    });
    
    updateScrollBar();
}

void HexEditor::setBinaryFile(BinaryFile* file) {
    m_binaryFile = file;
    m_editHistory.clear();
    m_firstVisibleAddress = 0;
    m_cursorAddress = 0;
    updateScrollBar();
    update();
}

void HexEditor::refresh() {
    update();
}

void HexEditor::updateScrollBar() {
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        m_scrollBar->setRange(0, 0);
        return;
    }
    
    size_t totalLines = (m_binaryFile->size() + m_bytesPerLine - 1) / m_bytesPerLine;
    int visibleLines = height() / m_lineHeight;
    int maxValue = static_cast<int>(totalLines) - visibleLines;
    if (maxValue < 0) maxValue = 0;
    
    m_scrollBar->setRange(0, maxValue);
    m_scrollBar->setPageStep(visibleLines);
    m_scrollBar->setSingleStep(1);
    m_scrollBar->setValue(static_cast<int>(m_firstVisibleAddress / m_bytesPerLine));
    
    m_scrollBar->setGeometry(width() - m_scrollBar->width(), 0, 
                            m_scrollBar->width(), height());
}

void HexEditor::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setFont(m_font);
    
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        painter.fillRect(rect(), palette().color(QPalette::Base));
        painter.setPen(palette().color(QPalette::Text));
        painter.drawText(rect(), Qt::AlignCenter, "No binary file loaded");
        return;
    }
    
    painter.fillRect(rect(), palette().color(QPalette::Base));
    
    int y = m_lineHeight;
    size_t address = m_firstVisibleAddress;
    size_t fileSize = m_binaryFile->size();
    
    // Draw address column background
    painter.fillRect(0, 0, ADDRESS_WIDTH, height(), palette().color(QPalette::AlternateBase));
    
    while (y < height() && address < fileSize) {
        // Draw address
        std::ostringstream addrStr;
        addrStr << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << address;
        painter.setPen(palette().color(QPalette::Text));
        painter.drawText(5, y, QString::fromStdString(addrStr.str()));
        
        // Draw hex area
        int x = ADDRESS_WIDTH + 10;
        for (int i = 0; i < m_bytesPerLine && address + i < fileSize; ++i) {
            size_t byteAddr = address + i;
            uint8_t byte = m_binaryFile->readByte(byteAddr);
            
            QRect byteRect = getByteRect(byteAddr);
            
            // Highlight cursor
            if (byteAddr == m_cursorAddress && hasFocus()) {
                painter.fillRect(byteRect, palette().color(QPalette::Highlight));
            }
            
            // Draw hex value
            std::ostringstream hexStr;
            hexStr << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
            painter.setPen(byteAddr == m_cursorAddress && hasFocus() ? 
                          palette().color(QPalette::HighlightedText) : 
                          palette().color(QPalette::Text));
            painter.drawText(x, y, QString::fromStdString(hexStr.str()));
            x += m_charWidth * 3;
        }
        
        // Draw ASCII area
        x = ADDRESS_WIDTH + HEX_AREA_WIDTH + 10;
        painter.setPen(palette().color(QPalette::Text));
        painter.drawText(x, y, "|");
        x += m_charWidth * 2;
        
        for (int i = 0; i < m_bytesPerLine && address + i < fileSize; ++i) {
            size_t byteAddr = address + i;
            uint8_t byte = m_binaryFile->readByte(byteAddr);
            char c = (byte >= 32 && byte < 127) ? static_cast<char>(byte) : '.';
            
            QRect byteRect(x - m_charWidth / 2, y - m_lineHeight + 4, m_charWidth, m_lineHeight);
            
            if (byteAddr == m_cursorAddress && hasFocus()) {
                painter.fillRect(byteRect, palette().color(QPalette::Highlight));
                painter.setPen(palette().color(QPalette::HighlightedText));
            } else {
                painter.setPen(palette().color(QPalette::Text));
            }
            
            painter.drawText(x, y, QString(c));
            x += m_charWidth;
        }
        
        address += m_bytesPerLine;
        y += m_lineHeight;
    }
}

void HexEditor::mousePressEvent(QMouseEvent* event) {
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return;
    }
    
    size_t address = posToAddress(event->pos());
    if (address < m_binaryFile->size()) {
        setCursorAddress(address);
        update();
    }
    
    setFocus();
}

void HexEditor::mouseMoveEvent(QMouseEvent* event) {
    // Could implement selection/drag here
    QWidget::mouseMoveEvent(event);
}

void HexEditor::setReadOnly(bool readOnly) {
    m_readOnly = readOnly;
    setToolTip(readOnly ? "Hex editing disabled in Safe Mode" : "");
    update();
}

void HexEditor::keyPressEvent(QKeyEvent* event) {
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        QWidget::keyPressEvent(event);
        return;
    }
    
    // Block editing when read-only (Safe Mode)
    if (m_readOnly) {
        QWidget::keyPressEvent(event);
        return;
    }
    
    size_t fileSize = m_binaryFile->size();
    
    switch (event->key()) {
        case Qt::Key_Left:
            if (m_cursorAddress > 0) {
                setCursorAddress(m_cursorAddress - 1);
            }
            break;
        case Qt::Key_Right:
            if (m_cursorAddress < fileSize - 1) {
                setCursorAddress(m_cursorAddress + 1);
            }
            break;
        case Qt::Key_Up:
            if (m_cursorAddress >= m_bytesPerLine) {
                setCursorAddress(m_cursorAddress - m_bytesPerLine);
            }
            break;
        case Qt::Key_Down:
            if (m_cursorAddress + m_bytesPerLine < fileSize) {
                setCursorAddress(m_cursorAddress + m_bytesPerLine);
            }
            break;
        case Qt::Key_PageUp:
            setCursorAddress(m_cursorAddress >= m_bytesPerLine * 10 ? 
                           m_cursorAddress - m_bytesPerLine * 10 : 0);
            break;
        case Qt::Key_PageDown:
            setCursorAddress(m_cursorAddress + m_bytesPerLine * 10 < fileSize ? 
                           m_cursorAddress + m_bytesPerLine * 10 : fileSize - 1);
            break;
        case Qt::Key_Home:
            setCursorAddress((m_cursorAddress / m_bytesPerLine) * m_bytesPerLine);
            break;
        case Qt::Key_End:
            {
                size_t targetAddr = (m_cursorAddress / m_bytesPerLine + 1) * m_bytesPerLine - 1;
                setCursorAddress((targetAddr < fileSize) ? targetAddr : fileSize - 1);
            }
            break;
        case Qt::Key_0: case Qt::Key_1: case Qt::Key_2: case Qt::Key_3: case Qt::Key_4:
        case Qt::Key_5: case Qt::Key_6: case Qt::Key_7: case Qt::Key_8: case Qt::Key_9:
        case Qt::Key_A: case Qt::Key_B: case Qt::Key_C: case Qt::Key_D: case Qt::Key_E: case Qt::Key_F:
            if (m_hexArea) {
                QString text = event->text();
                bool ok;
                int digit = text.toInt(&ok, 16);
                if (ok) {
                    // Simple implementation - just replace the byte
                    uint8_t currentByte = m_binaryFile->readByte(m_cursorAddress);
                    uint8_t newByte = (currentByte << 4) | digit;
                    m_editHistory.pushEdit(m_cursorAddress, currentByte, newByte);
                    m_binaryFile->writeByte(m_cursorAddress, newByte);
                    emit dataChanged();
                    update();
                }
            }
            break;
        case Qt::Key_Backspace:
        case Qt::Key_Delete:
            // Could implement delete
            break;
        case Qt::Key_Z:
            if (event->modifiers() & Qt::ControlModifier) {
                if (event->modifiers() & Qt::ShiftModifier) {
                    redo();
                } else {
                    undo();
                }
            }
            break;
        default:
            QWidget::keyPressEvent(event);
            return;
    }
    
    update();
}

void HexEditor::wheelEvent(QWheelEvent* event) {
    int delta = event->angleDelta().y();
    if (delta > 0) {
        m_scrollBar->setValue(m_scrollBar->value() - 3);
    } else {
        m_scrollBar->setValue(m_scrollBar->value() + 3);
    }
}

void HexEditor::resizeEvent(QResizeEvent* event) {
    updateScrollBar();
    QWidget::resizeEvent(event);
}

void HexEditor::setCursorAddress(size_t address) {
    if (!m_binaryFile || address >= m_binaryFile->size()) {
        return;
    }
    
    m_cursorAddress = address;
    
    // Scroll into view
    if (m_cursorAddress < m_firstVisibleAddress) {
        m_firstVisibleAddress = (m_cursorAddress / m_bytesPerLine) * m_bytesPerLine;
        m_scrollBar->setValue(static_cast<int>(m_firstVisibleAddress / m_bytesPerLine));
    } else {
        int visibleLines = height() / m_lineHeight;
        size_t lastVisible = m_firstVisibleAddress + visibleLines * m_bytesPerLine;
        if (m_cursorAddress >= lastVisible) {
            m_firstVisibleAddress = ((m_cursorAddress / m_bytesPerLine) - visibleLines + 1) * m_bytesPerLine;
            m_scrollBar->setValue(static_cast<int>(m_firstVisibleAddress / m_bytesPerLine));
        }
    }
    
    emit addressChanged(m_cursorAddress);
}

size_t HexEditor::posToAddress(const QPoint& pos) const {
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return 0;
    }
    
    int line = (pos.y() - m_lineHeight) / m_lineHeight;
    if (line < 0) line = 0;
    
    size_t address = m_firstVisibleAddress + line * m_bytesPerLine;
    
    if (pos.x() < ADDRESS_WIDTH + HEX_AREA_WIDTH) {
        // Hex area
        int byteIndex = (pos.x() - ADDRESS_WIDTH - 10) / (m_charWidth * 3);
        if (byteIndex < 0) byteIndex = 0;
        if (byteIndex >= m_bytesPerLine) byteIndex = m_bytesPerLine - 1;
        address += byteIndex;
    } else {
        // ASCII area
        int byteIndex = (pos.x() - ADDRESS_WIDTH - HEX_AREA_WIDTH - 10 - m_charWidth * 2) / m_charWidth;
        if (byteIndex < 0) byteIndex = 0;
        if (byteIndex >= m_bytesPerLine) byteIndex = m_bytesPerLine - 1;
        address += byteIndex;
    }
    
    if (address >= m_binaryFile->size()) {
        address = m_binaryFile->size() - 1;
    }
    
    return address;
}

QRect HexEditor::getByteRect(size_t address) const {
    size_t line = (address - m_firstVisibleAddress) / m_bytesPerLine;
    size_t byteInLine = (address - m_firstVisibleAddress) % m_bytesPerLine;
    
    int y = static_cast<int>(line + 1) * m_lineHeight;
    int x = ADDRESS_WIDTH + 10 + static_cast<int>(byteInLine) * m_charWidth * 3;
    
    return QRect(x, y - m_lineHeight + 4, m_charWidth * 2, m_lineHeight);
}

bool HexEditor::canUndo() const {
    return m_editHistory.canUndo();
}

bool HexEditor::canRedo() const {
    return m_editHistory.canRedo();
}

void HexEditor::undo() {
    if (!m_binaryFile || !canUndo()) {
        return;
    }
    
    auto edit = m_editHistory.undo();
    m_binaryFile->writeByte(edit.offset, edit.oldValue);
    m_cursorAddress = edit.offset;
    emit dataChanged();
    update();
}

void HexEditor::redo() {
    if (!m_binaryFile || !canRedo()) {
        return;
    }
    
    auto edit = m_editHistory.redo();
    m_binaryFile->writeByte(edit.offset, edit.newValue);
    m_cursorAddress = edit.offset;
    emit dataChanged();
    update();
}

void HexEditor::goToAddress(size_t address) {
    setCursorAddress(address);
}

void HexEditor::setAddress(size_t address) {
    setCursorAddress(address);
}

} // namespace WinMMM10

