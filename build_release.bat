@echo off
echo Building WinMMM10 Editor...
echo.

REM Check if build directory exists, if not create it
if not exist "build" mkdir build
cd build

REM Configure CMake - Try multiple configurations
echo Configuring CMake...
echo Trying Visual Studio 2022 with v143 toolset...
cmake .. -G "Visual Studio 17 2022" -A x64 -T v143
if errorlevel 1 (
    echo.
    echo v143 toolset not found. Trying v142 toolset (VS 2019 compatible)...
    cmake .. -G "Visual Studio 17 2022" -A x64 -T v142
    if errorlevel 1 (
        echo.
        echo v142 toolset not found. Trying Visual Studio 2019...
        cmake .. -G "Visual Studio 16 2019" -A x64
        if errorlevel 1 (
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
    )
)

REM Build in Release mode
echo.
echo Building Release configuration...
cmake --build . --config Release
if errorlevel 1 (
    echo.
    echo Build failed!
    echo Check the error messages above for details.
    cd ..
    pause
    exit /b 1
)

echo.
echo ============================================
echo Build completed successfully!
echo ============================================
echo Executable location: build\Release\WinMMM10Editor.exe
echo.
cd ..
pause

