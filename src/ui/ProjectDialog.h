#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

namespace WinMMM10 {

class ProjectDialog : public QDialog {
    Q_OBJECT

public:
    explicit ProjectDialog(QWidget* parent = nullptr);
    ~ProjectDialog() override = default;
    
    QString projectName() const;
    QString ecuName() const;
    QString description() const;
    QString binaryFilePath() const;
    
    void setProjectName(const QString& name);
    void setEcuName(const QString& name);
    void setDescription(const QString& desc);
    void setBinaryFilePath(const QString& path);

private slots:
    void onBrowseBinary();

private:
    QLineEdit* m_nameEdit{nullptr};
    QLineEdit* m_ecuEdit{nullptr};
    QTextEdit* m_descriptionEdit{nullptr};
    QLineEdit* m_binaryPathEdit{nullptr};
    QPushButton* m_browseButton{nullptr};
};

} // namespace WinMMM10

