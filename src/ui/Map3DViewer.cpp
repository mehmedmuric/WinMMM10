#include "Map3DViewer.h"
#include <QMatrix4x4>
#include <QtMath>

namespace WinMMM10 {

Map3DViewer::Map3DViewer(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_binaryFile(nullptr)
{
    setMinimumSize(400, 300);
}

void Map3DViewer::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat lightPos[4] = {2.0f, 2.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void Map3DViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Map3DViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (m_map.rows() == 0 || m_map.columns() == 0) {
        return;
    }
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = static_cast<float>(width()) / static_cast<float>(height());
    float fov = 45.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;
    float f = 1.0f / tan(fov * 0.5f * 3.14159265359f / 180.0f);
    
    float proj[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zFar + zNear) / (zNear - zFar), -1,
        0, 0, (2.0f * zFar * zNear) / (zNear - zFar), 0
    };
    glLoadMatrixf(proj);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -m_distance);
    glRotatef(m_rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(m_rotationY, 0.0f, 1.0f, 0.0f);
    
    drawSurface();
}

void Map3DViewer::drawSurface() {
    size_t rows = m_map.rows();
    size_t cols = m_map.columns();
    
    if (rows == 0 || cols == 0) {
        return;
    }
    
    // Normalize coordinates to [-1, 1] range for display
    double xScale = 2.0 / static_cast<double>(cols);
    double yScale = 2.0 / static_cast<double>(rows);
    double zRange = m_maxValue - m_minValue;
    if (zRange < 0.0001) zRange = 1.0;
    double zScale = 2.0 / zRange;
    double zOffset = -(m_minValue + m_maxValue) / 2.0;
    
    glBegin(GL_TRIANGLES);
    
    for (size_t row = 0; row < rows - 1; ++row) {
        for (size_t col = 0; col < cols - 1; ++col) {
            double x1 = (static_cast<double>(col) - cols/2.0) * xScale;
            double y1 = (static_cast<double>(row) - rows/2.0) * yScale;
            double z1 = (m_map.getPhysicalValue(row, col) + zOffset) * zScale;
            
            double x2 = (static_cast<double>(col + 1) - cols/2.0) * xScale;
            double y2 = (static_cast<double>(row) - rows/2.0) * yScale;
            double z2 = (m_map.getPhysicalValue(row, col + 1) + zOffset) * zScale;
            
            double x3 = (static_cast<double>(col) - cols/2.0) * xScale;
            double y3 = (static_cast<double>(row + 1) - rows/2.0) * yScale;
            double z3 = (m_map.getPhysicalValue(row + 1, col) + zOffset) * zScale;
            
            double x4 = (static_cast<double>(col + 1) - cols/2.0) * xScale;
            double y4 = (static_cast<double>(row + 1) - rows/2.0) * yScale;
            double z4 = (m_map.getPhysicalValue(row + 1, col + 1) + zOffset) * zScale;
            
            // Normalize values for color
            QVector3D color1 = getColorForValue(m_map.getPhysicalValue(row, col), m_minValue, m_maxValue);
            QVector3D color2 = getColorForValue(m_map.getPhysicalValue(row, col + 1), m_minValue, m_maxValue);
            QVector3D color3 = getColorForValue(m_map.getPhysicalValue(row + 1, col), m_minValue, m_maxValue);
            QVector3D color4 = getColorForValue(m_map.getPhysicalValue(row + 1, col + 1), m_minValue, m_maxValue);
            
            // First triangle
            glColor3f(color1.x(), color1.y(), color1.z());
            glVertex3f(static_cast<GLfloat>(x1), static_cast<GLfloat>(y1), static_cast<GLfloat>(z1));
            glColor3f(color2.x(), color2.y(), color2.z());
            glVertex3f(static_cast<GLfloat>(x2), static_cast<GLfloat>(y2), static_cast<GLfloat>(z2));
            glColor3f(color3.x(), color3.y(), color3.z());
            glVertex3f(static_cast<GLfloat>(x3), static_cast<GLfloat>(y3), static_cast<GLfloat>(z3));
            
            // Second triangle
            glColor3f(color2.x(), color2.y(), color2.z());
            glVertex3f(static_cast<GLfloat>(x2), static_cast<GLfloat>(y2), static_cast<GLfloat>(z2));
            glColor3f(color4.x(), color4.y(), color4.z());
            glVertex3f(static_cast<GLfloat>(x4), static_cast<GLfloat>(y4), static_cast<GLfloat>(z4));
            glColor3f(color3.x(), color3.y(), color3.z());
            glVertex3f(static_cast<GLfloat>(x3), static_cast<GLfloat>(y3), static_cast<GLfloat>(z3));
        }
    }
    
    glEnd();
}

QVector3D Map3DViewer::getColorForValue(double value, double minVal, double maxVal) {
    double normalized = (value - minVal) / (maxVal - minVal);
    if (normalized < 0.0) normalized = 0.0;
    if (normalized > 1.0) normalized = 1.0;
    
    // Blue to red gradient
    if (normalized < 0.5) {
        // Blue to green
        float t = static_cast<float>(normalized * 2.0);
        return QVector3D(0.0f, t, 1.0f - t);
    } else {
        // Green to red
        float t = static_cast<float>((normalized - 0.5) * 2.0);
        return QVector3D(t, 1.0f - t, 0.0f);
    }
}

void Map3DViewer::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_lastMousePos = event->pos();
        m_mousePressed = true;
    }
    QOpenGLWidget::mousePressEvent(event);
}

void Map3DViewer::mouseMoveEvent(QMouseEvent* event) {
    if (m_mousePressed) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_rotationY += delta.x() * 0.5f;
        m_rotationX += delta.y() * 0.5f;
        if (m_rotationX > 90.0f) m_rotationX = 90.0f;
        if (m_rotationX < -90.0f) m_rotationX = -90.0f;
        m_lastMousePos = event->pos();
        update();
    }
    QOpenGLWidget::mouseMoveEvent(event);
}

void Map3DViewer::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = false;
    }
    QOpenGLWidget::mouseReleaseEvent(event);
}

void Map3DViewer::wheelEvent(QWheelEvent* event) {
    float delta = event->angleDelta().y() / 120.0f;
    m_distance -= delta * 0.5f;
    if (m_distance < 1.0f) m_distance = 1.0f;
    if (m_distance > 20.0f) m_distance = 20.0f;
    update();
    QOpenGLWidget::wheelEvent(event);
}

void Map3DViewer::setMap(const MapDefinition& definition, BinaryFile* file) {
    m_binaryFile = file;
    m_map = Map3D(definition);
    
    if (!m_binaryFile || !m_binaryFile->isLoaded()) {
        return;
    }
    
    size_t address = definition.address();
    size_t dataSize = definition.totalSize();
    
    if (address + dataSize > m_binaryFile->size()) {
        return;
    }
    
    const uint8_t* data = m_binaryFile->at(address);
    m_map.loadFromBinary(data, dataSize);
    
    // Calculate min/max
    m_minValue = m_maxValue = m_map.getPhysicalValue(0, 0);
    for (size_t row = 0; row < m_map.rows(); ++row) {
        for (size_t col = 0; col < m_map.columns(); ++col) {
            double val = m_map.getPhysicalValue(row, col);
            if (val < m_minValue) m_minValue = val;
            if (val > m_maxValue) m_maxValue = val;
        }
    }
    
    updateMap();
}

void Map3DViewer::updateMap() {
    update();
}

} // namespace WinMMM10
