#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include "../maps/Map2D.h"
#include "../binary/BinaryFile.h"

namespace WinMMM10 {

class Map2DViewer : public QWidget {
    Q_OBJECT

public:
    explicit Map2DViewer(QWidget* parent = nullptr);
    ~Map2DViewer() override = default;
    
    void setMap(const MapDefinition& definition, BinaryFile* file);
    void updateMap();

signals:
    void mapChanged();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Map2D m_map;
    BinaryFile* m_binaryFile{nullptr};
    QLabel* m_infoLabel{nullptr};
};

} // namespace WinMMM10
