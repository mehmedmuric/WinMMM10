#include "ProjectDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>

namespace WinMMM10 {

ProjectDialog::ProjectDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("New Project");
    setModal(true);
    
    auto* layout = new QFormLayout(this);
    
    m_nameEdit = new QLineEdit();
    layout->addRow("Project Name:", m_nameEdit);
    
    m_ecuEdit = new QLineEdit();
    layout->addRow("ECU Name:", m_ecuEdit);
    
    m_descriptionEdit = new QTextEdit();
    m_descriptionEdit->setMaximumHeight(100);
    layout->addRow("Description:", m_descriptionEdit);
    
    auto* binaryLayout = new QHBoxLayout();
    m_binaryPathEdit = new QLineEdit();
    m_browseButton = new QPushButton("Browse...");
    connect(m_browseButton, &QPushButton::clicked, this, &ProjectDialog::onBrowseBinary);
    binaryLayout->addWidget(m_binaryPathEdit);
    binaryLayout->addWidget(m_browseButton);
    layout->addRow("Binary File:", binaryLayout);
    
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addRow(buttonBox);
}

QString ProjectDialog::projectName() const {
    return m_nameEdit->text();
}

QString ProjectDialog::ecuName() const {
    return m_ecuEdit->text();
}

QString ProjectDialog::description() const {
    return m_descriptionEdit->toPlainText();
}

QString ProjectDialog::binaryFilePath() const {
    return m_binaryPathEdit->text();
}

void ProjectDialog::setProjectName(const QString& name) {
    m_nameEdit->setText(name);
}

void ProjectDialog::setEcuName(const QString& name) {
    m_ecuEdit->setText(name);
}

void ProjectDialog::setDescription(const QString& desc) {
    m_descriptionEdit->setPlainText(desc);
}

void ProjectDialog::setBinaryFilePath(const QString& path) {
    m_binaryPathEdit->setText(path);
}

void ProjectDialog::onBrowseBinary() {
    QString filename = QFileDialog::getOpenFileName(this, "Select Binary File", "", "Binary Files (*.bin *.hex);;All Files (*.*)");
    if (!filename.isEmpty()) {
        m_binaryPathEdit->setText(filename);
    }
}

} // namespace WinMMM10

