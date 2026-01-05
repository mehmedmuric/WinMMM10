@echo off
setlocal enabledelayedexpansion

echo ============================================
echo Building WinMMM10 Editor (Release)
echo ============================================
echo.

REM Record start time
set START_TIME=%TIME%
set START_MS=%TIME:~-2%

REM Check if build directory exists, if not create it
if not exist "build" mkdir build
cd build

REM Optional clean build
set CLEAN_BUILD=0
if "%1"=="--clean" (
    set CLEAN_BUILD=1
    echo Performing clean build (removing CMakeCache.txt)...
    if exist CMakeCache.txt del /Q CMakeCache.txt
    if exist CMakeFiles rmdir /S /Q CMakeFiles
    echo.
)

REM Configure CMake - Try multiple configurations
echo [1/2] Configuring CMake...
echo.

set CMAKE_CONFIG_SUCCESS=0

echo Trying Visual Studio 2022 with v143 toolset...
cmake .. -G "Visual Studio 17 2022" -A x64 -T v143 -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo.
    echo v143 toolset not found. Trying v142 toolset (VS 2019 compatible)...
    cmake .. -G "Visual Studio 17 2022" -A x64 -T v142 -DCMAKE_BUILD_TYPE=Release
    if errorlevel 1 (
        echo.
        echo v142 toolset not found. Trying Visual Studio 2019...
        cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release
        if errorlevel 1 (
            set CMAKE_CONFIG_SUCCESS=1
        )
    )
)

if !CMAKE_CONFIG_SUCCESS!==1 (
    echo.
    echo ============================================
    echo CMake configuration failed!
    echo ============================================
    echo.
    echo SOLUTION: Install C++ build tools in Visual Studio
    echo.
    echo Option 1 (Recommended):
    echo   1. Open Visual Studio Installer
    echo   2. Click "Modify" on Visual Studio 2022
    echo   3. Select "Desktop development with C++" workload
    echo   4. Click "Modify" to install
    echo   5. Run this script again
    echo.
    echo Option 2:
    echo   Install Visual Studio 2019 with C++ tools
    echo.
    cd ..
    pause
    exit /b 1
)

echo.
echo Configuration successful!
echo Build type: Release
echo.

REM Build in Release mode with parallel compilation
echo [2/2] Building Release configuration (parallel compilation)...
echo.

REM Use parallel builds (--parallel uses all available cores)
cmake --build . --config Release --parallel
if errorlevel 1 (
    echo.
    echo ============================================
    echo Build failed!
    echo ============================================
    echo Check the error messages above for details.
    echo.
    cd ..
    pause
    exit /b 1
)

REM Calculate elapsed time
set END_TIME=%TIME%
set END_MS=%END_TIME:~-2%

echo.
echo ============================================
echo Build completed successfully!
echo ============================================
echo.
echo Configuration: Release
echo Build system: Visual Studio (Multi-config)
echo Parallel compilation: Enabled
echo.

REM Try to display executable location (may vary based on generator)
if exist "Release\WinMMM10Editor.exe" (
    echo Executable location: build\Release\WinMMM10Editor.exe
    echo.
) else if exist "x64\Release\WinMMM10Editor.exe" (
    echo Executable location: build\x64\Release\WinMMM10Editor.exe
    echo.
) else (
    echo Executable location: Check build directory for WinMMM10Editor.exe
    echo.
)

echo Start time: %START_TIME%
echo End time: %END_TIME%
echo.

cd ..
pause
