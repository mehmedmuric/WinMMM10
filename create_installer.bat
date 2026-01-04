@echo off
echo Creating WinMMM10 Editor Installer...
echo.

REM Check if executable exists
if not exist "build\Release\WinMMM10Editor.exe" (
    echo Error: WinMMM10Editor.exe not found!
    echo Please build the project first using build_release.bat
    pause
    exit /b 1
)

REM Create dist directory if it doesn't exist
if not exist "dist" mkdir dist

REM Check if Inno Setup is installed
set INNO_SETUP=C:\Program Files (x86)\Inno Setup 6\ISCC.exe
if not exist "%INNO_SETUP%" (
    set INNO_SETUP=C:\Program Files\Inno Setup 6\ISCC.exe
)

if not exist "%INNO_SETUP%" (
    echo.
    echo ERROR: Inno Setup Compiler not found!
    echo Please install Inno Setup from: https://jrsoftware.org/isdl.php
    echo Or update the INNO_SETUP path in this script.
    echo.
    pause
    exit /b 1
)

REM Compile the installer
echo Compiling installer script...
"%INNO_SETUP%" "installer\WinMMM10Editor.iss"

if errorlevel 1 (
    echo.
    echo Installer compilation failed!
    echo Please check the installer script and Qt paths.
    pause
    exit /b 1
)

echo.
echo Installer created successfully!
echo Location: dist\WinMMM10Editor_Setup.exe
echo.
echo You can now copy this file to a USB drive for distribution.
echo.
pause

