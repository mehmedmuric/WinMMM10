#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMatrix4x4>
#include <QVector3D>
#include "../maps/Map3D.h"
#include "../binary/BinaryFile.h"

namespace WinMMM10 {

class Map3DViewer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Map3DViewer(QWidget* parent = nullptr);
    ~Map3DViewer() override = default;
    
    void setMap(const MapDefinition& definition, BinaryFile* file);
    void updateMap();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void drawSurface();
    QVector3D getColorForValue(double value, double minVal, double maxVal);
    
    Map3D m_map;
    BinaryFile* m_binaryFile{nullptr};
    
    float m_rotationX{30.0f};
    float m_rotationY{45.0f};
    float m_distance{5.0f};
    QPoint m_lastMousePos;
    bool m_mousePressed{false};
    
    double m_minValue{0.0};
    double m_maxValue{100.0};
};

} // namespace WinMMM10

