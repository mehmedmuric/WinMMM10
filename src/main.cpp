#include "pch.h"
#include "core/Application.h"
#include "ui/MainWindow.h"
#include "ui/DarkTheme.h"
#include <QMessageBox>
#include <QDebug>
#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        qDebug() << "Starting WinMMM10 Editor...";
        
        QApplication app(argc, argv);
        qDebug() << "QApplication created";
        
        app.setApplicationName(WinMMM10::Application::applicationName());
        app.setOrganizationName(WinMMM10::Application::organizationName());
        app.setApplicationVersion(WinMMM10::Application::version());
        qDebug() << "Application properties set";
        
        qDebug() << "Applying dark theme...";
        WinMMM10::DarkTheme::apply(&app);
        qDebug() << "Theme applied";
        
        qDebug() << "Creating MainWindow...";
        WinMMM10::MainWindow window;
        qDebug() << "MainWindow created successfully";
        
        qDebug() << "Showing window...";
        window.show();
        qDebug() << "Window shown, entering event loop";
        
        return app.exec();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        QMessageBox::critical(nullptr, "Fatal Error", 
            QString("Application crashed:\n%1").arg(e.what()));
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
        QMessageBox::critical(nullptr, "Fatal Error", 
            "Application crashed with unknown exception");
        return 1;
    }
}

