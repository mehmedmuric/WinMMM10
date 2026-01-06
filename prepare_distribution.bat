@echo off
setlocal ENABLEEXTENSIONS

echo ============================================
echo WinMMM10 Editor - Distribution Preparation
echo ============================================
echo.

REM === CLEAN BUILD CACHE ===
echo [1/4] Cleaning build cache...
if exist "out" (
    rmdir /s /q "out" 2>nul
    echo   Build output directory removed
)
if exist "CMakeCache.txt" (
    del /q "CMakeCache.txt" 2>nul
    echo   CMake cache removed
)
if exist "CMakeFiles" (
    rmdir /s /q "CMakeFiles" 2>nul
    echo   CMakeFiles directory removed
)
if exist ".vs" (
    rmdir /s /q ".vs" 2>nul
    echo   Visual Studio cache removed
)
echo   [OK] Build cache cleaned
echo.

REM === CLEAN DISTRIBUTION DIRECTORIES ===
echo [2/4] Cleaning distribution directories...
if exist "dist" (
    rmdir /s /q "dist" 2>nul
    echo   Distribution directory removed
)
if exist "installer\dist" (
    rmdir /s /q "installer\dist" 2>nul
    echo   Installer dist directory removed
)
echo   [OK] Distribution directories cleaned
echo.

REM === CLEAN TEMPORARY FILES ===
echo [3/4] Cleaning temporary files...
for /r %%f in (*.log *.tmp *.user *.suo) do (
    if exist "%%f" (
        del /q "%%f" 2>nul
    )
)
echo   [OK] Temporary files cleaned
echo.

REM === VERIFY INSTALLER SCRIPT ===
echo [4/4] Verifying installer configuration...
if exist "installer\WinMMM10Editor.iss" (
    echo   [OK] Installer script found
    echo   [INFO] Make sure QtRoot path in installer script matches your Qt installation
    echo   [INFO] Current path: C:\Qt\6.10.1\msvc2022_64
) else (
    echo   [WARNING] Installer script not found!
)
echo.

echo ============================================
echo [SUCCESS] Project ready for distribution!
echo ============================================
echo.
echo Next steps:
echo   1. Build the project: build_release.bat
echo   2. Create installer: create_installer.bat
echo   3. Test installer on clean system
echo   4. Distribute: dist\WinMMM10Editor_Setup.exe
echo.
echo The installer includes all required DLLs and dependencies.
echo.
pause

