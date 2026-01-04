#include "core/Application.h"
#include "ui/MainWindow.h"
#include "ui/DarkTheme.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName(WinMMM10::Application::applicationName());
    app.setOrganizationName(WinMMM10::Application::organizationName());
    app.setApplicationVersion(WinMMM10::Application::version());
    
    WinMMM10::DarkTheme::apply(&app);
    
    WinMMM10::MainWindow window;
    window.show();
    
    return app.exec();
}

