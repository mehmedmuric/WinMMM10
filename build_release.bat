@echo off
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

echo ============================================
echo Building WinMMM10 Editor (Release)
echo ============================================
echo.

REM === PROJECT ROOT ===
cd /d C:\WinMMM10 || exit /b 1

REM === CLEAN BUILD ===
if exist out (
    echo Cleaning previous build...
    rmdir /s /q out
)

REM === QT CONFIG ===
set QT_ROOT=C:\Qt\6.10.1\msvc2022_64
set Qt6_DIR=%QT_ROOT%\lib\cmake\Qt6

echo Using Qt6 from:
echo %Qt6_DIR%
echo.

REM === CONFIGURE CMAKE ===
echo [1/2] Configuring CMake...

cmake -S . -B out\build\x64-Release ^
 -G "Visual Studio 18 2026" ^
 -A x64 ^
 -T v145 ^
 -DCMAKE_PREFIX_PATH="%QT_ROOT%\lib\cmake"

IF ERRORLEVEL 1 (
    echo.
    echo ❌ CMake configuration failed
    pause
    exit /b 1
)

echo Configuration successful!
echo.

REM === BUILD ===
echo [2/2] Building Release...

cmake --build out\build\x64-Release --config Release --parallel

IF ERRORLEVEL 1 (
    echo.
    echo ❌ Build failed
    pause
    exit /b 1
)

echo.
echo ============================================
echo ✅ Build completed successfully!
echo ============================================

echo Executable location:
echo C:\WinMMM10\out\build\x64-Release\WinMMM10Editor.exe
echo.

pause
