#include "pch.h"
#include "core/Application.h"
#include "ui/MainWindow.h"
#include "ui/DarkTheme.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

namespace {
    // Global log file stream
    std::ofstream g_logFile;
    
    void logToFile(const std::string& message) {
        if (g_logFile.is_open()) {
#ifdef _WIN32
            SYSTEMTIME st;
            GetLocalTime(&st);
            char timestamp[64];
            sprintf_s(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                     st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
            g_logFile << "[" << timestamp << "] " << message << std::endl;
#else
            // For non-Windows, use simple timestamp
            time_t now = time(nullptr);
            char timestamp[64];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
            g_logFile << "[" << timestamp << "] " << message << std::endl;
#endif
            g_logFile.flush();
        }
    }
    
    void logToBoth(const std::string& message) {
        std::cout << message << std::endl;
        std::cerr << message << std::endl;
        logToFile(message);
    }
    
    void setupConsole() {
#ifdef _WIN32
        // Allocate console for GUI application
        if (AllocConsole()) {
            // Redirect stdout and stderr to console
            FILE* pCout;
            FILE* pCerr;
            FILE* pCin;
            
            freopen_s(&pCout, "CONOUT$", "w", stdout);
            freopen_s(&pCerr, "CONOUT$", "w", stderr);
            freopen_s(&pCin, "CONIN$", "r", stdin);
            
            // Set console title
            SetConsoleTitle(L"WinMMM10 Editor - Debug Console");
            
            std::cout.clear();
            std::cerr.clear();
            std::cin.clear();
        }
#endif
    }
    
    void setupLogFile() {
#ifdef _WIN32
        // Get executable directory using Windows API (before Qt initialization)
        char exePath[MAX_PATH];
        if (GetModuleFileNameA(nullptr, exePath, MAX_PATH) > 0) {
            std::string path(exePath);
            size_t lastSlash = path.find_last_of("\\/");
            if (lastSlash != std::string::npos) {
                path = path.substr(0, lastSlash + 1);
            }
            path += "WinMMM10Editor.log";
            g_logFile.open(path, std::ios::app);
            if (g_logFile.is_open()) {
                g_logFile << "[=== WinMMM10 Editor Session Started ===]" << std::endl;
                g_logFile.flush();
            }
        }
#else
        // For non-Windows, use current directory
        g_logFile.open("WinMMM10Editor.log", std::ios::app);
        if (g_logFile.is_open()) {
            g_logFile << "[=== WinMMM10 Editor Session Started ===]" << std::endl;
            g_logFile.flush();
        }
#endif
    }
}

int main(int argc, char* argv[]) {
    // Setup logging first (before Qt initialization)
    setupLogFile();
    setupConsole();
    
    logToBoth("Starting WinMMM10 Editor...");
    
#ifdef _WIN32
    // Windows SEH exception handling
    __try {
#endif
        try {
            logToBoth("Creating QApplication...");
            QApplication app(argc, argv);
            logToBoth("QApplication created");
            
            app.setApplicationName(WinMMM10::Application::applicationName());
            app.setOrganizationName(WinMMM10::Application::organizationName());
            app.setApplicationVersion(WinMMM10::Application::version());
            logToBoth("Application properties set");
            
            logToBoth("Applying dark theme...");
            WinMMM10::DarkTheme::apply(&app);
            logToBoth("Theme applied");
            
            logToBoth("Creating MainWindow...");
            WinMMM10::MainWindow window;
            logToBoth("MainWindow created successfully");
            
            logToBoth("Showing window...");
            window.show();
            logToBoth("Window shown, entering event loop");
            
            int result = app.exec();
            logToBoth("Application event loop exited with code: " + std::to_string(result));
            
            if (g_logFile.is_open()) {
                logToFile("=== WinMMM10 Editor Session Ended ===");
                g_logFile.close();
            }
            
            return result;
        }
        catch (const std::exception& e) {
            std::string errorMsg = "C++ Exception caught: " + std::string(e.what());
            logToBoth(errorMsg);
            
            if (QApplication::instance()) {
                QMessageBox::critical(nullptr, "Fatal Error", 
                    QString("Application crashed:\n%1").arg(e.what()));
            } else {
                // Qt not initialized, show in console only
                MessageBoxA(nullptr, errorMsg.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
            }
            
            if (g_logFile.is_open()) {
                logToFile("=== WinMMM10 Editor Session Crashed ===");
                g_logFile.close();
            }
            
            return 1;
        }
        catch (...) {
            std::string errorMsg = "Unknown C++ exception caught";
            logToBoth(errorMsg);
            
            if (QApplication::instance()) {
                QMessageBox::critical(nullptr, "Fatal Error", 
                    "Application crashed with unknown exception");
            } else {
                MessageBoxA(nullptr, errorMsg.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
            }
            
            if (g_logFile.is_open()) {
                logToFile("=== WinMMM10 Editor Session Crashed ===");
                g_logFile.close();
            }
            
            return 1;
        }
#ifdef _WIN32
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        DWORD exceptionCode = GetExceptionCode();
        std::stringstream ss;
        ss << "Windows SEH Exception caught: 0x" << std::hex << std::uppercase << exceptionCode;
        std::string errorMsg = ss.str();
        logToBoth(errorMsg);
        
        if (QApplication::instance()) {
            QMessageBox::critical(nullptr, "Fatal Error", 
                QString("Application crashed with Windows exception:\n%1").arg(QString::fromStdString(errorMsg)));
        } else {
            MessageBoxA(nullptr, errorMsg.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
        }
        
        if (g_logFile.is_open()) {
            logToFile("=== WinMMM10 Editor Session Crashed (SEH) ===");
            g_logFile.close();
        }
        
        return 1;
    }
#endif
}

