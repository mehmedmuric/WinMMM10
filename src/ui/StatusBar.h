#pragma once

#include <QStatusBar>
#include <QLabel>

namespace WinMMM10 {

class StatusBar : public QStatusBar {
    Q_OBJECT

public:
    explicit StatusBar(QWidget* parent = nullptr);
    ~StatusBar() override = default;
    
    void setAddress(size_t address);
    void setFileInfo(const QString& filename, size_t size);
    void setMessage(const QString& message);

private:
    QLabel* m_addressLabel{nullptr};
    QLabel* m_fileLabel{nullptr};
    QLabel* m_messageLabel{nullptr};
};

} // namespace WinMMM10

