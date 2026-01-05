#include "AnnotationsPanel.h"
#include <QMessageBox>
#include <QColorDialog>
#include <QGroupBox>
#include <QLabel>
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

AnnotationsPanel::AnnotationsPanel(QWidget* parent)
    : QDockWidget("Annotations", parent)
{
    setupUI();
}

void AnnotationsPanel::setupUI() {
    QWidget* widget = new QWidget(this);
    setWidget(widget);
    auto* layout = new QVBoxLayout(widget);
    
    // Annotation list
    m_annotationList = new QListWidget(this);
    m_annotationList->setAlternatingRowColors(true);
    connect(m_annotationList, &QListWidget::itemSelectionChanged, this, &AnnotationsPanel::onAnnotationSelected);
    connect(m_annotationList, &QListWidget::itemDoubleClicked, this, &AnnotationsPanel::onAnnotationDoubleClicked);
    layout->addWidget(m_annotationList);
    
    // Add annotation section
    auto* addGroup = new QGroupBox("Add Annotation", this);
    auto* addLayout = new QVBoxLayout(addGroup);
    
    auto* addrLayout = new QHBoxLayout();
    addrLayout->addWidget(new QLabel("Address:", this));
    m_addressEdit = new QLineEdit(this);
    m_addressEdit->setPlaceholderText("0x00000000");
    addrLayout->addWidget(m_addressEdit);
    addLayout->addLayout(addrLayout);
    
    m_noteEdit = new QTextEdit(this);
    m_noteEdit->setPlaceholderText("Enter annotation note...");
    m_noteEdit->setMaximumHeight(100);
    addLayout->addWidget(m_noteEdit);
    
    auto* colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Color:", this));
    m_colorButton = new QPushButton(this);
    m_colorButton->setFixedSize(30, 30);
    m_colorButton->setStyleSheet(QString("background-color: %1; border: 1px solid #666;").arg(m_currentColor.name()));
    connect(m_colorButton, &QPushButton::clicked, this, &AnnotationsPanel::onColorButtonClicked);
    colorLayout->addWidget(m_colorButton);
    colorLayout->addStretch();
    addLayout->addLayout(colorLayout);
    
    m_addButton = new QPushButton("Add Annotation", this);
    connect(m_addButton, &QPushButton::clicked, this, &AnnotationsPanel::onAddAnnotation);
    addLayout->addWidget(m_addButton);
    
    layout->addWidget(addGroup);
    
    // Delete button
    m_deleteButton = new QPushButton("Delete Selected", this);
    m_deleteButton->setEnabled(false);
    connect(m_deleteButton, &QPushButton::clicked, this, &AnnotationsPanel::onDeleteAnnotation);
    layout->addWidget(m_deleteButton);
}

void AnnotationsPanel::setAnnotationManager(AnnotationManager* manager) {
    m_annotationManager = manager;
    refreshAnnotations();
}

void AnnotationsPanel::refreshAnnotations() {
    populateAnnotations();
}

void AnnotationsPanel::populateAnnotations() {
    m_annotationList->clear();
    
    if (!m_annotationManager) {
        return;
    }
    
    auto annotations = m_annotationManager->getAllAnnotations();
    for (const auto& annotation : annotations) {
        std::ostringstream oss;
        oss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << annotation.address;
        QString text = QString::fromStdString(oss.str()) + ": " + QString::fromStdString(annotation.note);
        
        auto* item = new QListWidgetItem(text, m_annotationList);
        item->setData(Qt::UserRole, static_cast<qulonglong>(annotation.address));
        
        QColor color(QString::fromStdString(annotation.color));
        item->setBackground(color);
        item->setForeground(color.lightness() < 128 ? Qt::white : Qt::black);
    }
}

void AnnotationsPanel::setCurrentAddress(size_t address) {
    m_currentAddress = address;
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << address;
    m_addressEdit->setText(QString::fromStdString(oss.str()));
}

void AnnotationsPanel::onAddAnnotation() {
    if (!m_annotationManager) return;
    
    QString addrText = m_addressEdit->text().trimmed();
    bool ok;
    size_t address = 0;
    if (addrText.startsWith("0x") || addrText.startsWith("0X")) {
        address = addrText.toULongLong(&ok, 16);
    } else {
        address = addrText.toULongLong(&ok, 16);
        if (!ok) {
            address = addrText.toULongLong(&ok, 10);
        }
    }
    
    if (!ok) {
        QMessageBox::warning(this, "Invalid Address", "Please enter a valid address.");
        return;
    }
    
    QString note = m_noteEdit->toPlainText().trimmed();
    if (note.isEmpty()) {
        QMessageBox::warning(this, "Invalid Annotation", "Annotation note cannot be empty.");
        return;
    }
    
    Annotation annotation;
    annotation.address = address;
    annotation.note = note.toStdString();
    annotation.color = m_currentColor.name().toStdString();
    
    m_annotationManager->addAnnotation(annotation);
    refreshAnnotations();
    
    m_noteEdit->clear();
}

void AnnotationsPanel::onDeleteAnnotation() {
    auto* item = m_annotationList->currentItem();
    if (!item || !m_annotationManager) return;
    
    size_t address = item->data(Qt::UserRole).toULongLong();
    m_annotationManager->removeAnnotation(address);
    refreshAnnotations();
}

void AnnotationsPanel::onAnnotationSelected() {
    auto* item = m_annotationList->currentItem();
    m_deleteButton->setEnabled(item != nullptr);
    if (item) {
        size_t address = item->data(Qt::UserRole).toULongLong();
        emit annotationSelected(address);
    }
}

void AnnotationsPanel::onAnnotationDoubleClicked(QListWidgetItem* item) {
    if (item) {
        size_t address = item->data(Qt::UserRole).toULongLong();
        emit annotationDoubleClicked(address);
    }
}

void AnnotationsPanel::onColorButtonClicked() {
    QColor color = QColorDialog::getColor(m_currentColor, this, "Select Annotation Color");
    if (color.isValid()) {
        m_currentColor = color;
        m_colorButton->setStyleSheet(QString("background-color: %1; border: 1px solid #666;").arg(color.name()));
    }
}

QColor AnnotationsPanel::getCurrentColor() const {
    return m_currentColor;
}

} // namespace WinMMM10

