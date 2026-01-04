#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "HexEditor.h"
#include "../binary/BinaryFile.h"

namespace WinMMM10 {

class HexEditorWidget : public QWidget {
    Q_OBJECT

public:
    explicit HexEditorWidget(QWidget* parent = nullptr);
    ~HexEditorWidget() override = default;
    
    void setBinaryFile(BinaryFile* file);
    HexEditor* hexEditor() { return m_hexEditor; }

private slots:
    void onGoToClicked();
    void onAddressChanged(size_t address);

private:
    HexEditor* m_hexEditor{nullptr};
    QLineEdit* m_addressEdit{nullptr};
    QLabel* m_statusLabel{nullptr};
};

} // namespace WinMMM10

