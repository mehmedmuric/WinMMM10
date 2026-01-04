#pragma once

#include <QListWidget>
#include <QContextMenuEvent>
#include "../maps/MapDefinition.h"

namespace WinMMM10 {

class MapListWidget : public QListWidget {
    Q_OBJECT

public:
    explicit MapListWidget(QWidget* parent = nullptr);
    ~MapListWidget() override = default;
    
    void addMap(const MapDefinition& map);
    void clearMaps();
    int currentMapIndex() const { return currentRow(); }

signals:
    void mapSelected(int index);
    void mapDoubleClicked(int index);
    void mapDeleteRequested(int index);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private slots:
    void onItemSelectionChanged();
    void onDeleteMap();
};

} // namespace WinMMM10

