#pragma once

#include <QDockWidget>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QColorDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../core/AnnotationManager.h"

namespace WinMMM10 {

class AnnotationsPanel : public QDockWidget {
    Q_OBJECT

public:
    explicit AnnotationsPanel(QWidget* parent = nullptr);
    ~AnnotationsPanel() override = default;
    
    void setAnnotationManager(AnnotationManager* manager);
    void refreshAnnotations();
    void setCurrentAddress(size_t address);

signals:
    void annotationSelected(size_t address);
    void annotationDoubleClicked(size_t address);

private slots:
    void onAddAnnotation();
    void onDeleteAnnotation();
    void onAnnotationSelected();
    void onAnnotationDoubleClicked(QListWidgetItem* item);
    void onColorButtonClicked();

private:
    void setupUI();
    void populateAnnotations();
    QColor getCurrentColor() const;
    
    AnnotationManager* m_annotationManager{nullptr};
    QListWidget* m_annotationList{nullptr};
    QLineEdit* m_addressEdit{nullptr};
    QTextEdit* m_noteEdit{nullptr};
    QPushButton* m_colorButton{nullptr};
    QPushButton* m_addButton{nullptr};
    QPushButton* m_deleteButton{nullptr};
    QColor m_currentColor{Qt::yellow};
    size_t m_currentAddress{0};
};

} // namespace WinMMM10

