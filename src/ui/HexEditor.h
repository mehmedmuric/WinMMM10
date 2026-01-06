#pragma once

#include <QWidget>
#include <QScrollBar>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFont>
#include <cstdint>
#include <cstddef>
#include "../binary/BinaryFile.h"
#include "../binary/EditHistory.h"

namespace WinMMM10 {

class HexEditor : public QWidget {
    Q_OBJECT

public:
    explicit HexEditor(QWidget* parent = nullptr);
    ~HexEditor() override = default;
    
    void setBinaryFile(BinaryFile* file);
    BinaryFile* binaryFile() const { return m_binaryFile; }
    
    void setAddress(size_t address);
    size_t currentAddress() const { return m_cursorAddress; }
    
    void refresh();
    
    bool canUndo() const;
    bool canRedo() const;
    
    void setReadOnly(bool readOnly);
    bool isReadOnly() const { return m_readOnly; }
    
public slots:
    void undo();
    void redo();
    void goToAddress(size_t address);
    
signals:
    void addressChanged(size_t address);
    void dataChanged();
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateScrollBar();
    QPoint addressToPos(size_t address) const;
    size_t posToAddress(const QPoint& pos) const;
    QRect getByteRect(size_t address) const;
    void setCursorAddress(size_t address);
    void insertByte(uint8_t value);
    void deleteByte();
    
    BinaryFile* m_binaryFile{nullptr};
    EditHistory m_editHistory;
    
    size_t m_firstVisibleAddress{0};
    size_t m_cursorAddress{0};
    bool m_hexArea{true}; // true = hex area, false = ascii area
    bool m_insertMode{false};
    bool m_readOnly{false};
    
    QScrollBar* m_scrollBar{nullptr};
    
    static constexpr int BYTES_PER_LINE = 16;
    static constexpr int ADDRESS_WIDTH = 80;
    static constexpr int HEX_AREA_WIDTH = 480;
    static constexpr int ASCII_AREA_WIDTH = 160;
    static constexpr int LINE_HEIGHT = 20;
    
    int m_bytesPerLine{BYTES_PER_LINE};
    int m_lineHeight{LINE_HEIGHT};
    int m_charWidth{0};
    QFont m_font;
};

} // namespace WinMMM10

