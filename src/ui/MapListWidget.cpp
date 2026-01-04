#include "MapListWidget.h"
#include <QMenu>
#include <QMessageBox>

namespace WinMMM10 {

MapListWidget::MapListWidget(QWidget* parent)
    : QListWidget(parent)
{
    connect(this, &QListWidget::itemSelectionChanged, this, &MapListWidget::onItemSelectionChanged);
}

void MapListWidget::addMap(const MapDefinition& map) {
    QString text = QString::fromStdString(map.name());
    text += " [" + QString::number(map.rows()) + "x" + QString::number(map.columns()) + "]";
    text += " @ 0x" + QString::number(map.address(), 16).toUpper();
    
    addItem(text);
}

void MapListWidget::clearMaps() {
    clear();
}

void MapListWidget::contextMenuEvent(QContextMenuEvent* event) {
    QListWidgetItem* item = itemAt(event->pos());
    if (!item) {
        return;
    }
    
    QMenu menu(this);
    QAction* deleteAction = menu.addAction("Delete");
    connect(deleteAction, &QAction::triggered, this, &MapListWidget::onDeleteMap);
    
    menu.exec(event->globalPos());
}

void MapListWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    QListWidget::mouseDoubleClickEvent(event);
    
    int index = currentRow();
    if (index >= 0) {
        emit mapDoubleClicked(index);
    }
}

void MapListWidget::onItemSelectionChanged() {
    int index = currentRow();
    if (index >= 0) {
        emit mapSelected(index);
    }
}

void MapListWidget::onDeleteMap() {
    int index = currentRow();
    if (index >= 0) {
        int ret = QMessageBox::question(this, "Delete Map", 
                                        "Are you sure you want to delete this map?",
                                        QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            emit mapDeleteRequested(index);
        }
    }
}

} // namespace WinMMM10

