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
#include "BookmarksPanel.h"
#include "AnnotationsPanel.h"
#include "RecentFilesMenu.h"
#include "SearchReplaceDialog.h"
#include "CacheSettingsDialog.h"
#include "../heuristics/MapDetector.h"
#include "../cache/CacheManager.h"
#include "../core/BookmarkManager.h"
#include "../core/AnnotationManager.h"
#include "../binary/HexSearch.h"
#include "../editing/MapComparator.h"
#include "../editing/BatchOperations.h"
#include "../editing/MapMath.h"
#include "../editing/InterpolationEngine.h"
#include "../kess/KessMapConverter.h"

namespace WinMMM10 {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

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
    void showCacheSettings();
    void showSearchReplace();
    void addBookmark();
    void addAnnotation();
    void compareMaps();
    void batchOperations();
    void mapMathOperations();
    void interpolateMap();
    void smoothMap();
    void importKessFile();
    void exportKessFile();
    void about();

private:
    void setupUI();
    void setupMenus();
    void setupToolbars();
    void setupDocks();
    void updateWindowTitle();
    bool maybeSave();
    void loadBinaryFile(const QString& filepath);
    void updateRecentFilesMenus();
    
    ProjectManager m_projectManager;
    BinaryFile m_binaryFile;
    MapDetector m_mapDetector;
    BookmarkManager m_bookmarkManager;
    AnnotationManager m_annotationManager;
    HexSearch* m_hexSearch{nullptr};
    MapComparator m_mapComparator;
    BatchOperations* m_batchOps{nullptr};
    MapMath* m_mapMath{nullptr};
    InterpolationEngine* m_interpolationEngine{nullptr};
    KessMapConverter m_kessConverter;
    
    // UI Components
    HexEditorWidget* m_hexEditor{nullptr};
    MapListWidget* m_mapList{nullptr};
    QTabWidget* m_mapViewerTabs{nullptr};
    Map2DViewer* m_map2DViewer{nullptr};
    Map3DViewer* m_map3DViewer{nullptr};
    StatusBar* m_statusBar{nullptr};
    BookmarksPanel* m_bookmarksPanel{nullptr};
    AnnotationsPanel* m_annotationsPanel{nullptr};
    RecentFilesMenu* m_recentProjectsMenu{nullptr};
    RecentFilesMenu* m_recentBinariesMenu{nullptr};
    SearchReplaceDialog* m_searchReplaceDialog{nullptr};
    
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
    QAction* m_searchAction{nullptr};
    QAction* m_addBookmarkAction{nullptr};
    QAction* m_addAnnotationAction{nullptr};
    QAction* m_compareMapsAction{nullptr};
    QAction* m_batchOpsAction{nullptr};
    QAction* m_mapMathAction{nullptr};
    QAction* m_interpolateAction{nullptr};
    QAction* m_smoothAction{nullptr};
    QAction* m_importKessAction{nullptr};
    QAction* m_exportKessAction{nullptr};
};

} // namespace WinMMM10

