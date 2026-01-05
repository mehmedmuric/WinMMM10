@echo off
setlocal ENABLEEXTENSIONS

echo ============================================
echo Cleaning WinMMM10 Editor Build Cache
echo ============================================
echo.

REM === PROJECT ROOT ===
cd /d C:\WinMMM10 || exit /b 1

echo Cleaning build artifacts and cache...
echo.

REM === REMOVE BUILD DIRECTORIES ===
if exist "out" (
    echo Removing build output directory (out)...
    rmdir /s /q "out" 2>nul
    if exist "out" (
        echo Warning: Could not fully remove 'out' directory. Some files may be locked.
    ) else (
        echo [OK] Removed 'out' directory
    )
)

if exist "build" (
    echo Removing build directory...
    rmdir /s /q "build" 2>nul
    if exist "build" (
        echo Warning: Could not fully remove 'build' directory. Some files may be locked.
    ) else (
        echo [OK] Removed 'build' directory
    )
)

REM === REMOVE CMake CACHE FILES ===
if exist "CMakeCache.txt" (
    del /q "CMakeCache.txt" 2>nul
    echo [OK] Removed CMakeCache.txt
)

if exist "cmake_install.cmake" (
    del /q "cmake_install.cmake" 2>nul
    echo [OK] Removed cmake_install.cmake
)

REM === REMOVE CMakeFiles DIRECTORY ===
if exist "CMakeFiles" (
    rmdir /s /q "CMakeFiles" 2>nul
    echo [OK] Removed CMakeFiles directory
)

REM === REMOVE VISUAL STUDIO CACHE ===
if exist ".vs" (
    echo Removing Visual Studio cache (.vs)...
    rmdir /s /q ".vs" 2>nul
    if exist ".vs" (
        echo Warning: Could not fully remove '.vs' directory. Close Visual Studio if open.
    ) else (
        echo [OK] Removed .vs directory
    )
)

REM === REMOVE USER FILES ===
for %%f in (*.user *.suo *.sdf *.opensdf) do (
    if exist "%%f" (
        del /q "%%f" 2>nul
        echo [OK] Removed %%f
    )
)

REM === REMOVE DIST DIRECTORY (optional - comment out if you want to keep installers) ===
REM Uncomment the following lines if you want to remove distribution files too:
REM if exist "dist" (
REM     echo Removing distribution directory...
REM     rmdir /s /q "dist" 2>nul
REM     echo ✓ Removed dist directory
REM )

echo.
echo ============================================
echo [SUCCESS] Cache cleanup completed!
echo ============================================
echo.
echo The project is now clean and ready for a fresh build.
echo Run 'build_release.bat' to build the project.
echo.
pause

