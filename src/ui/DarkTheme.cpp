#include "DarkTheme.h"

namespace WinMMM10 {

void DarkTheme::apply(QApplication* app) {
    app->setStyle(QStyleFactory::create("Fusion"));
    
    QPalette darkPalette;
    
    // Enhanced color scheme with better contrast
    QColor darkGray(45, 45, 48);
    QColor darkerGray(30, 30, 30);
    QColor lightGray(200, 200, 200);
    QColor blue(0, 120, 215);
    QColor darkBlue(0, 90, 158);
    QColor green(16, 124, 16);
    QColor orange(255, 140, 0);
    
    // Window colors
    darkPalette.setColor(QPalette::Window, darkGray);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, darkerGray);
    darkPalette.setColor(QPalette::AlternateBase, darkGray);
    
    // Text colors
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::BrightText, orange);
    darkPalette.setColor(QPalette::ToolTipBase, darkerGray);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    
    // Button colors
    darkPalette.setColor(QPalette::Button, darkGray);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    
    // Highlight colors
    darkPalette.setColor(QPalette::Highlight, blue);
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    
    // Link colors
    darkPalette.setColor(QPalette::Link, blue);
    darkPalette.setColor(QPalette::LinkVisited, darkBlue);
    
    // Disabled colors
    QColor disabledGray(100, 100, 100);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledGray);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledGray);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, disabledGray);
    
    app->setPalette(darkPalette);
    
    // Set stylesheet for additional styling
    app->setStyleSheet(
        "QToolTip {"
        "    color: #ffffff;"
        "    background-color: #2d2d30;"
        "    border: 1px solid #3e3e42;"
        "    padding: 4px;"
        "}"
        "QMenuBar {"
        "    background-color: #2d2d30;"
        "    color: #ffffff;"
        "    border-bottom: 1px solid #3e3e42;"
        "}"
        "QMenuBar::item:selected {"
        "    background-color: #0078d4;"
        "}"
        "QMenu {"
        "    background-color: #2d2d30;"
        "    color: #ffffff;"
        "    border: 1px solid #3e3e42;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #0078d4;"
        "}"
        "QToolBar {"
        "    background-color: #2d2d30;"
        "    border: none;"
        "    spacing: 4px;"
        "}"
        "QToolBar::separator {"
        "    background-color: #3e3e42;"
        "    width: 1px;"
        "    margin: 4px;"
        "}"
        "QStatusBar {"
        "    background-color: #0078d4;"
        "    color: #ffffff;"
        "    border-top: 1px solid #3e3e42;"
        "}"
        "QDockWidget {"
        "    titlebar-close-icon: url(:/icons/close.png);"
        "    titlebar-normal-icon: url(:/icons/float.png);"
        "}"
        "QDockWidget::title {"
        "    background-color: #2d2d30;"
        "    padding: 4px;"
        "    border-bottom: 1px solid #3e3e42;"
        "}"
    );
}

} // namespace WinMMM10

