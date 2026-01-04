#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QSplitter>
#include <QTabWidget>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include "../core/ProjectManager.h"
#include "../core/Settings.h"
#include "../binary/BinaryFile.h"
#include "HexEditorWidget.h"
#include "MapListWidget.h"
#include "Map2DViewer.h"
#include "Map3DViewer.h"
#include "StatusBar.h"
#include "ProjectDialog.h"
#include "MapDefinitionDialog.h"
#include "../heuristics/MapDetector.h"

namespace WinMMM10 {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();
    void loadBinary();
    void saveBinary();
    void saveBinaryAs();
    void detectMaps();
    void addMap();
    void editMap();
    void deleteMap();
    void onMapSelected(int index);
    void onMapDoubleClicked(int index);
    void exportMapDefinitions();
    void importMapDefinitions();
    void about();

private:
    void setupUI();
    void setupMenus();
    void setupToolbars();
    void setupDocks();
    void updateWindowTitle();
    bool maybeSave();
    void loadBinaryFile(const QString& filepath);
    
    ProjectManager m_projectManager;
    BinaryFile m_binaryFile;
    MapDetector m_mapDetector;
    
    // UI Components
    HexEditorWidget* m_hexEditor{nullptr};
    MapListWidget* m_mapList{nullptr};
    QTabWidget* m_mapViewerTabs{nullptr};
    Map2DViewer* m_map2DViewer{nullptr};
    Map3DViewer* m_map3DViewer{nullptr};
    StatusBar* m_statusBar{nullptr};
    
    // Actions
    QAction* m_newProjectAction{nullptr};
    QAction* m_openProjectAction{nullptr};
    QAction* m_saveProjectAction{nullptr};
    QAction* m_loadBinaryAction{nullptr};
    QAction* m_saveBinaryAction{nullptr};
    QAction* m_detectMapsAction{nullptr};
    QAction* m_addMapAction{nullptr};
    QAction* m_editMapAction{nullptr};
    QAction* m_deleteMapAction{nullptr};
    QAction* m_exitAction{nullptr};
};

} // namespace WinMMM10

