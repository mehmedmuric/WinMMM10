#pragma once

// =====================
// Qt Core
// =====================
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QFile>
#include <QDir>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>

// =====================
// Qt Gui
// =====================
#include <QFont>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QMatrix4x4>
#include <QVector3D>

// =====================
// Qt Widgets
// =====================
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QSplitter>
#include <QTabWidget>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyleFactory>

// =====================
// Qt OpenGL (only for main app, not tests)
// =====================
#ifndef WINMMM10_TESTS
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#endif

// Standard Library
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

