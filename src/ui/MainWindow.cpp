#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    Settings::instance().load();
    
    setupUI();
    setupMenus();
    setupToolbars();
    setupDocks();
    
    updateWindowTitle();
    setMinimumSize(1024, 768);
    resize(1280, 800);
}

void MainWindow::setupUI() {
    // Central widget with hex editor
    m_hexEditor = new HexEditorWidget(this);
    setCentralWidget(m_hexEditor);
    
    // Status bar
    m_statusBar = new StatusBar(this);
    setStatusBar(m_statusBar);
}

void MainWindow::setupMenus() {
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");
    
    m_newProjectAction = fileMenu->addAction("&New Project", this, &MainWindow::newProject, QKeySequence::New);
    m_openProjectAction = fileMenu->addAction("&Open Project...", this, &MainWindow::openProject, QKeySequence::Open);
    fileMenu->addSeparator();
    m_saveProjectAction = fileMenu->addAction("&Save Project", this, &MainWindow::saveProject, QKeySequence::Save);
    m_saveProjectAction->setEnabled(false);
    fileMenu->addAction("Save Project &As...", this, &MainWindow::saveProjectAs, QKeySequence::SaveAs);
    fileMenu->addSeparator();
    m_loadBinaryAction = fileMenu->addAction("&Load Binary...", this, &MainWindow::loadBinary);
    m_saveBinaryAction = fileMenu->addAction("&Save Binary", this, &MainWindow::saveBinary, QKeySequence("Ctrl+Alt+S"));
    m_saveBinaryAction->setEnabled(false);
    fileMenu->addAction("Save Binary &As...", this, &MainWindow::saveBinaryAs);
    fileMenu->addSeparator();
    m_exitAction = fileMenu->addAction("E&xit", this, &QWidget::close, QKeySequence::Quit);
    
    // Map menu
    QMenu* mapMenu = menuBar()->addMenu("&Map");
    m_detectMapsAction = mapMenu->addAction("&Detect Maps...", this, &MainWindow::detectMaps);
    m_detectMapsAction->setEnabled(false);
    mapMenu->addSeparator();
    m_addMapAction = mapMenu->addAction("&Add Map...", this, &MainWindow::addMap);
    m_addMapAction->setEnabled(false);
    m_editMapAction = mapMenu->addAction("&Edit Map...", this, &MainWindow::editMap);
    m_editMapAction->setEnabled(false);
    m_deleteMapAction = mapMenu->addAction("&Delete Map", this, &MainWindow::deleteMap);
    m_deleteMapAction->setEnabled(false);
    mapMenu->addSeparator();
    mapMenu->addAction("&Export Definitions...", this, &MainWindow::exportMapDefinitions);
    mapMenu->addAction("&Import Definitions...", this, &MainWindow::importMapDefinitions);
    
    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("&About", this, &MainWindow::about);
}

void MainWindow::setupToolbars() {
    QToolBar* mainToolbar = addToolBar("Main");
    mainToolbar->addAction(m_newProjectAction);
    mainToolbar->addAction(m_openProjectAction);
    mainToolbar->addAction(m_saveProjectAction);
    mainToolbar->addSeparator();
    mainToolbar->addAction(m_loadBinaryAction);
    mainToolbar->addAction(m_saveBinaryAction);
    mainToolbar->addSeparator();
    mainToolbar->addAction(m_detectMapsAction);
    mainToolbar->addAction(m_addMapAction);
}

void MainWindow::setupDocks() {
    // Maps dock
    QDockWidget* mapsDock = new QDockWidget("Maps", this);
    mapsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_mapList = new MapListWidget(mapsDock);
    connect(m_mapList, &MapListWidget::mapSelected, this, &MainWindow::onMapSelected);
    connect(m_mapList, &MapListWidget::mapDoubleClicked, this, &MainWindow::onMapDoubleClicked);
    connect(m_mapList, &MapListWidget::mapDeleteRequested, this, &MainWindow::deleteMap);
    mapsDock->setWidget(m_mapList);
    addDockWidget(Qt::RightDockWidgetArea, mapsDock);
    
    // Map viewer dock
    QDockWidget* viewerDock = new QDockWidget("Map Viewer", this);
    viewerDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    m_mapViewerTabs = new QTabWidget(viewerDock);
    m_map2DViewer = new Map2DViewer(m_mapViewerTabs);
    m_map3DViewer = new Map3DViewer(m_mapViewerTabs);
    m_mapViewerTabs->addTab(m_map2DViewer, "2D View");
    m_mapViewerTabs->addTab(m_map3DViewer, "3D View");
    viewerDock->setWidget(m_mapViewerTabs);
    addDockWidget(Qt::BottomDockWidgetArea, viewerDock);
}

void MainWindow::updateWindowTitle() {
    QString title = "WinMMM10 Editor";
    if (m_projectManager.hasCurrentProject()) {
        title += " - " + QString::fromStdString(m_projectManager.currentProject()->name());
        if (m_projectManager.hasUnsavedChanges()) {
            title += "*";
        }
    }
    setWindowTitle(title);
}

void MainWindow::newProject() {
    if (!maybeSave()) {
        return;
    }
    
    ProjectDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString filepath = QFileDialog::getSaveFileName(this, "Save Project", "", "Project Files (*.wmm10)");
        if (!filepath.isEmpty()) {
            m_projectManager.createProject(filepath.toStdString(), dialog.projectName().toStdString());
            m_projectManager.currentProject()->setEcuName(dialog.ecuName().toStdString());
            m_projectManager.currentProject()->setDescription(dialog.description().toStdString());
            m_projectManager.currentProject()->setBinaryFilepath(dialog.binaryFilePath().toStdString());
            
            loadBinaryFile(dialog.binaryFilePath());
            
            updateWindowTitle();
            m_saveProjectAction->setEnabled(true);
            m_addMapAction->setEnabled(true);
            m_detectMapsAction->setEnabled(m_binaryFile.isLoaded());
            
            // Clear maps
            m_mapList->clearMaps();
            m_map2DViewer->setMap(MapDefinition(), nullptr);
            m_map3DViewer->setMap(MapDefinition(), nullptr);
        }
    }
}

void MainWindow::openProject() {
    if (!maybeSave()) {
        return;
    }
    
    QString filepath = QFileDialog::getOpenFileName(this, "Open Project", "", "Project Files (*.wmm10)");
    if (!filepath.isEmpty()) {
        if (m_projectManager.loadProject(filepath.toStdString())) {
            Project* project = m_projectManager.currentProject();
            
            // Load binary file
            if (!project->binaryFilepath().empty()) {
                loadBinaryFile(QString::fromStdString(project->binaryFilepath()));
            }
            
            // Load maps
            m_mapList->clearMaps();
            for (size_t i = 0; i < project->mapCount(); ++i) {
                m_mapList->addMap(project->getMap(i));
            }
            
            updateWindowTitle();
            m_saveProjectAction->setEnabled(true);
            m_addMapAction->setEnabled(true);
            m_detectMapsAction->setEnabled(m_binaryFile.isLoaded());
        } else {
            QMessageBox::critical(this, "Error", "Failed to open project file.");
        }
    }
}

void MainWindow::saveProject() {
    if (m_projectManager.saveProject()) {
        updateWindowTitle();
    } else {
        QMessageBox::critical(this, "Error", "Failed to save project.");
    }
}

void MainWindow::saveProjectAs() {
    QString filepath = QFileDialog::getSaveFileName(this, "Save Project As", "", "Project Files (*.wmm10)");
    if (!filepath.isEmpty()) {
        if (m_projectManager.saveProjectAs(filepath.toStdString())) {
            updateWindowTitle();
        } else {
            QMessageBox::critical(this, "Error", "Failed to save project.");
        }
    }
}

void MainWindow::loadBinary() {
    QString filepath = QFileDialog::getOpenFileName(this, "Load Binary File", "", "Binary Files (*.bin *.hex);;All Files (*.*)");
    if (!filepath.isEmpty()) {
        loadBinaryFile(filepath);
        if (m_projectManager.hasCurrentProject()) {
            m_projectManager.currentProject()->setBinaryFilepath(filepath.toStdString());
        }
    }
}

void MainWindow::loadBinaryFile(const QString& filepath) {
    if (m_binaryFile.load(filepath.toStdString())) {
        m_hexEditor->setBinaryFile(&m_binaryFile);
        m_statusBar->setFileInfo(QFileInfo(filepath).fileName(), m_binaryFile.size());
        m_saveBinaryAction->setEnabled(true);
        m_detectMapsAction->setEnabled(true);
        
        Settings::instance().setLastBinaryPath(filepath.toStdString());
        Settings::instance().save();
    } else {
        QMessageBox::critical(this, "Error", "Failed to load binary file.");
    }
}

void MainWindow::saveBinary() {
    if (m_binaryFile.save()) {
        m_statusBar->setMessage("Binary file saved.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to save binary file.");
    }
}

void MainWindow::saveBinaryAs() {
    QString filepath = QFileDialog::getSaveFileName(this, "Save Binary File As", "", "Binary Files (*.bin);;All Files (*.*)");
    if (!filepath.isEmpty()) {
        if (m_binaryFile.save(filepath.toStdString())) {
            m_statusBar->setMessage("Binary file saved.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to save binary file.");
        }
    }
}

void MainWindow::detectMaps() {
    if (!m_binaryFile.isLoaded()) {
        return;
    }
    
    m_mapDetector.setBinaryData(m_binaryFile.data(), m_binaryFile.size());
    auto candidates = m_mapDetector.detectMaps();
    
    if (candidates.empty()) {
        QMessageBox::information(this, "Map Detection", "No maps detected.");
        return;
    }
    
    QStringList items;
    for (const auto& candidate : candidates) {
        QString item = QString("0x%1 - %2x%3 (confidence: %4%)")
                      .arg(candidate.address, 0, 16)
                      .arg(candidate.rows)
                      .arg(candidate.columns)
                      .arg(candidate.confidence * 100, 0, 'f', 1);
        items << item;
    }
    
    bool ok;
    QString selected = QInputDialog::getItem(this, "Select Map", "Detected maps:", items, 0, false, &ok);
    if (ok && !selected.isEmpty()) {
        int index = items.indexOf(selected);
        if (index >= 0 && index < static_cast<int>(candidates.size())) {
            const auto& candidate = candidates[index];
            
            MapDefinition map;
            map.setName(QString("Detected Map %1").arg(index + 1).toStdString());
            map.setAddress(candidate.address);
            map.setType(candidate.type);
            map.setRows(candidate.rows);
            map.setColumns(candidate.columns);
            map.setDataType(2); // uint16 default
            
            if (m_projectManager.hasCurrentProject()) {
                m_projectManager.currentProject()->addMap(map);
                m_mapList->addMap(map);
                m_projectManager.markChanged();
                updateWindowTitle();
            }
        }
    }
}

void MainWindow::addMap() {
    if (!m_projectManager.hasCurrentProject()) {
        return;
    }
    
    MapDefinition map = MapDefinitionDialog::getMapDefinition(this);
    if (!map.name().empty()) {
        m_projectManager.currentProject()->addMap(map);
        m_mapList->addMap(map);
        m_projectManager.markChanged();
        updateWindowTitle();
    }
}

void MainWindow::editMap() {
    int index = m_mapList->currentMapIndex();
    if (index < 0 || !m_projectManager.hasCurrentProject()) {
        return;
    }
    
    Project* project = m_projectManager.currentProject();
    MapDefinition map = project->getMap(index);
    
    MapDefinitionDialog dialog(this);
    dialog.setMapDefinition(map);
    if (dialog.exec() == QDialog::Accepted) {
        map = dialog.getMapDefinition();
        project->getMap(index) = map;
        
        m_mapList->clearMaps();
        for (size_t i = 0; i < project->mapCount(); ++i) {
            m_mapList->addMap(project->getMap(i));
        }
        m_mapList->setCurrentRow(index);
        
        m_projectManager.markChanged();
        updateWindowTitle();
    }
}

void MainWindow::deleteMap() {
    int index = m_mapList->currentMapIndex();
    if (index < 0 || !m_projectManager.hasCurrentProject()) {
        return;
    }
    
    m_projectManager.currentProject()->removeMap(index);
    m_mapList->clearMaps();
    
    Project* project = m_projectManager.currentProject();
    for (size_t i = 0; i < project->mapCount(); ++i) {
        m_mapList->addMap(project->getMap(i));
    }
    
    m_projectManager.markChanged();
    updateWindowTitle();
}

void MainWindow::onMapSelected(int index) {
    if (index < 0 || !m_projectManager.hasCurrentProject() || !m_binaryFile.isLoaded()) {
        return;
    }
    
    Project* project = m_projectManager.currentProject();
    const MapDefinition& map = project->getMap(index);
    
    if (map.type() == MapType::Map2D) {
        m_map2DViewer->setMap(map, &m_binaryFile);
        m_mapViewerTabs->setCurrentIndex(0);
    } else {
        m_map3DViewer->setMap(map, &m_binaryFile);
        m_mapViewerTabs->setCurrentIndex(1);
    }
    
    m_editMapAction->setEnabled(true);
    m_deleteMapAction->setEnabled(true);
}

void MainWindow::onMapDoubleClicked(int index) {
    editMap();
}

void MainWindow::exportMapDefinitions() {
    // Implementation for exporting map definitions
    QMessageBox::information(this, "Export", "Export functionality to be implemented.");
}

void MainWindow::importMapDefinitions() {
    // Implementation for importing map definitions
    QMessageBox::information(this, "Import", "Import functionality to be implemented.");
}

void MainWindow::about() {
    QMessageBox::about(this, "About WinMMM10 Editor",
                      "WinMMM10 Editor v1.0.0\n\n"
                      "Professional ECU binary map editor\n"
                      "for automotive tuning and research.\n\n"
                      "Copyright (c) 2024");
}

bool MainWindow::maybeSave() {
    if (m_projectManager.hasUnsavedChanges()) {
        int ret = QMessageBox::warning(this, "Unsaved Changes",
                                      "The project has been modified.\n"
                                      "Do you want to save your changes?",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            saveProject();
            return true;
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (maybeSave()) {
        Settings::instance().save();
        event->accept();
    } else {
        event->ignore();
    }
}

} // namespace WinMMM10

