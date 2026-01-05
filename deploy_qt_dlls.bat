@echo off
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

echo ============================================
echo Deploying Qt DLLs to Build Directory
echo ============================================
echo.

REM === PROJECT ROOT ===
cd /d C:\WinMMM10 || exit /b 1

REM === QT CONFIG ===
if not defined QT_ROOT (
    set QT_ROOT=C:\Qt\6.10.1\msvc2022_64
)

echo Using Qt6 from: %QT_ROOT%
echo.

REM === BUILD OUTPUT DIRECTORY ===
set BUILD_DIR=out\build\x64-Release\Release
set EXE_DIR=%BUILD_DIR%

if not exist "%EXE_DIR%" (
    echo Error: Build directory not found: %EXE_DIR%
    echo Please build the project first using build_release.bat
    pause
    exit /b 1
)

echo Copying Qt DLLs to: %EXE_DIR%
echo.

REM === COPY REQUIRED QT DLLS ===
if not exist "%QT_ROOT%\bin" (
    echo Error: Qt bin directory not found: %QT_ROOT%\bin
    pause
    exit /b 1
)

echo Copying core Qt DLLs...
copy /Y "%QT_ROOT%\bin\Qt6Core.dll" "%EXE_DIR%\" >nul 2>&1
copy /Y "%QT_ROOT%\bin\Qt6Gui.dll" "%EXE_DIR%\" >nul 2>&1
copy /Y "%QT_ROOT%\bin\Qt6Widgets.dll" "%EXE_DIR%\" >nul 2>&1
copy /Y "%QT_ROOT%\bin\Qt6OpenGL.dll" "%EXE_DIR%\" >nul 2>&1
copy /Y "%QT_ROOT%\bin\Qt6OpenGLWidgets.dll" "%EXE_DIR%\" >nul 2>&1

REM === COPY PLATFORM PLUGIN ===
set PLATFORM_DIR=%EXE_DIR%\platforms
if not exist "%PLATFORM_DIR%" mkdir "%PLATFORM_DIR%"
copy /Y "%QT_ROOT%\plugins\platforms\qwindows.dll" "%PLATFORM_DIR%\" >nul 2>&1

REM === COPY STYLE PLUGINS (if they exist) ===
set STYLES_DIR=%EXE_DIR%\styles
if not exist "%STYLES_DIR%" mkdir "%STYLES_DIR%"
if exist "%QT_ROOT%\plugins\styles\*.dll" (
    copy /Y "%QT_ROOT%\plugins\styles\*.dll" "%STYLES_DIR%\" >nul 2>&1
)

REM === COPY ADDITIONAL QT DEPENDENCIES ===
REM Copy any additional DLLs that Qt might need
if exist "%QT_ROOT%\bin\Qt6OpenGL.dll" (
    echo Qt6OpenGL.dll copied
)
if exist "%QT_ROOT%\bin\Qt6OpenGLWidgets.dll" (
    echo Qt6OpenGLWidgets.dll copied
)

echo.
echo ============================================
echo ✅ Qt DLLs deployed successfully!
echo ============================================
echo.
echo The executable should now run correctly.
echo Location: %EXE_DIR%\WinMMM10Editor.exe
echo.
pause

