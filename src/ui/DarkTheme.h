#pragma once

#include <QString>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>

namespace WinMMM10 {

class DarkTheme {
public:
    static void apply(QApplication* app) {
        app->setStyle(QStyleFactory::create("Fusion"));
        
        QPalette darkPalette;
        
        // Colors
        QColor darkGray(53, 53, 53);
        QColor gray(128, 128, 128);
        QColor lightGray(189, 189, 189);
        QColor blue(42, 130, 218);
        
        darkPalette.setColor(QPalette::Window, darkGray);
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, darkGray);
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, darkGray);
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Link, blue);
        darkPalette.setColor(QPalette::Highlight, blue);
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, gray);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, gray);
        
        app->setPalette(darkPalette);
    }
};

} // namespace WinMMM10

