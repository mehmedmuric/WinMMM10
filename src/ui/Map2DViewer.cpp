#include "Map2DViewer.h"
#include <QMessageBox>
#include <QPainter>
#include <QtMath>

namespace WinMMM10 {

Map2DViewer::Map2DViewer(QWidget* parent)
    : QWidget(parent)
    , m_binaryFile(nullptr)
{
    auto* layout = new QVBoxLayout(this);
    
    m_infoLabel = new QLabel("No map loaded");
    layout->addWidget(m_infoLabel);
    
    // Simple plot widget using QWidget with custom painting
    // For production, consider using QCustomPlot or Qt Charts
    setMinimumSize(400, 300);
    
    setLayout(layout);
}

void Map2DViewer::setMap(const MapDefinition& definition, BinaryFile* file) {
    m_binaryFile = file;
    m_map = Map2D(definition);
    
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        m_infoLabel->setText("No binary file loaded");
        return;
    }
    
    // Load map data
    size_t address = definition.address();
    size_t dataSize = definition.totalSize();
    
    if (address + dataSize > m_binaryFile->size()) {
        m_infoLabel->setText("Map address out of range");
        return;
    }
    
    const uint8_t* data = m_binaryFile->at(address);
    m_map.loadFromBinary(data, dataSize);
    
    updateMap();
}

void Map2DViewer::updateMap() {
    size_t pointCount = m_map.pointCount();
    if (pointCount == 0) {
        m_infoLabel->setText("No data points");
        return;
    }
    
    double minX = 0, maxX = 0, minY = 0, maxY = 0;
    
    for (size_t i = 0; i < pointCount; ++i) {
        double x = m_map.getXAxisValue(i);
        double y = m_map.getPhysicalValue(i);
        
        if (i == 0) {
            minX = maxX = x;
            minY = maxY = y;
        } else {
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }
    }
    
    QString info = QString("Points: %1, Range X: [%2, %3], Range Y: [%4, %5]")
                   .arg(pointCount)
                   .arg(minX, 0, 'f', 2)
                   .arg(maxX, 0, 'f', 2)
                   .arg(minY, 0, 'f', 2)
                   .arg(maxY, 0, 'f', 2);
    m_infoLabel->setText(info);
    
    update(); // Trigger repaint
}

void Map2DViewer::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if (m_map.pointCount() == 0) {
        return;
    }
    
    QRect plotRect = rect().adjusted(50, 50, -50, -50);
    if (plotRect.width() < 100 || plotRect.height() < 100) {
        return;
    }
    
    // Calculate ranges
    double minX = 0, maxX = 0, minY = 0, maxY = 0;
    size_t pointCount = m_map.pointCount();
    
    for (size_t i = 0; i < pointCount; ++i) {
        double x = m_map.getXAxisValue(i);
        double y = m_map.getPhysicalValue(i);
        
        if (i == 0) {
            minX = maxX = x;
            minY = maxY = y;
        } else {
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }
    }
    
    double xRange = maxX - minX;
    double yRange = maxY - minY;
    if (xRange < 0.0001) xRange = 1.0;
    if (yRange < 0.0001) yRange = 1.0;
    
    // Draw axes
    painter.setPen(palette().color(QPalette::Text));
    painter.drawLine(plotRect.left(), plotRect.bottom(), plotRect.right(), plotRect.bottom());
    painter.drawLine(plotRect.left(), plotRect.top(), plotRect.left(), plotRect.bottom());
    
    // Draw curve
    QPen pen(palette().color(QPalette::Highlight), 2);
    painter.setPen(pen);
    
    QPointF prevPoint;
    bool first = true;
    
    for (size_t i = 0; i < pointCount; ++i) {
        double x = m_map.getXAxisValue(i);
        double y = m_map.getPhysicalValue(i);
        
        double normX = (x - minX) / xRange;
        double normY = 1.0 - (y - minY) / yRange; // Flip Y
        
        QPointF point(plotRect.left() + normX * plotRect.width(),
                     plotRect.top() + normY * plotRect.height());
        
        if (!first) {
            painter.drawLine(prevPoint, point);
        }
        
        prevPoint = point;
        first = false;
    }
    
    // Draw points
    painter.setBrush(palette().color(QPalette::Highlight));
    painter.setPen(Qt::NoPen);
    for (size_t i = 0; i < pointCount; ++i) {
        double x = m_map.getXAxisValue(i);
        double y = m_map.getPhysicalValue(i);
        
        double normX = (x - minX) / xRange;
        double normY = 1.0 - (y - minY) / yRange;
        
        QPointF point(plotRect.left() + normX * plotRect.width(),
                     plotRect.top() + normY * plotRect.height());
        
        painter.drawEllipse(point, 3, 3);
    }
}

} // namespace WinMMM10
