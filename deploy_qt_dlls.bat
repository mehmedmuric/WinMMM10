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
set EXE_PATH=%EXE_DIR%\WinMMM10Editor.exe

if not exist "%EXE_PATH%" (
    echo Error: Executable not found: %EXE_PATH%
    echo Please build the project first using build_release.bat
    pause
    exit /b 1
)

REM === CHECK FOR WINDEPLOYQT ===
set WINDEPLOYQT=%QT_ROOT%\bin\windeployqt.exe

if exist "%WINDEPLOYQT%" (
    echo Using Qt windeployqt tool for automatic deployment...
    echo.
    
    REM Use windeployqt to automatically deploy all required DLLs
    "%WINDEPLOYQT%" --release --compiler-runtime --no-translations --no-system-d3d-compiler --no-opengl-sw "%EXE_PATH%"
    
    IF ERRORLEVEL 1 (
        echo.
        echo Warning: windeployqt failed, falling back to manual deployment...
        goto :manual_deploy
    ) else (
        echo.
        echo ============================================
        echo ✅ Qt DLLs deployed successfully using windeployqt!
        echo ============================================
        echo.
        echo The executable should now run correctly.
        echo Location: %EXE_PATH%
        echo.
        goto :end
    )
) else (
    echo windeployqt not found, using manual deployment...
    goto :manual_deploy
)

:manual_deploy
echo.
echo Copying Qt DLLs manually...
echo.

REM === COPY REQUIRED QT DLLS ===
if not exist "%QT_ROOT%\bin" (
    echo Error: Qt bin directory not found: %QT_ROOT%\bin
    pause
    exit /b 1
)

echo Copying core Qt DLLs...
copy /Y "%QT_ROOT%\bin\Qt6Core.dll" "%EXE_DIR%\" >nul 2>&1 && echo   Qt6Core.dll
copy /Y "%QT_ROOT%\bin\Qt6Gui.dll" "%EXE_DIR%\" >nul 2>&1 && echo   Qt6Gui.dll
copy /Y "%QT_ROOT%\bin\Qt6Widgets.dll" "%EXE_DIR%\" >nul 2>&1 && echo   Qt6Widgets.dll
copy /Y "%QT_ROOT%\bin\Qt6OpenGL.dll" "%EXE_DIR%\" >nul 2>&1 && echo   Qt6OpenGL.dll
copy /Y "%QT_ROOT%\bin\Qt6OpenGLWidgets.dll" "%EXE_DIR%\" >nul 2>&1 && echo   Qt6OpenGLWidgets.dll
copy /Y "%QT_ROOT%\bin\Qt6Svg.dll" "%EXE_DIR%\" >nul 2>&1 && echo   Qt6Svg.dll

REM === COPY ICU DLLS (International Components for Unicode) ===
echo Copying ICU DLLs...
if exist "%QT_ROOT%\bin\icuuc*.dll" (
    for %%f in ("%QT_ROOT%\bin\icuuc*.dll") do (
        copy /Y "%%f" "%EXE_DIR%\" >nul 2>&1 && echo   %%~nxf
    )
)
if exist "%QT_ROOT%\bin\icudt*.dll" (
    for %%f in ("%QT_ROOT%\bin\icudt*.dll") do (
        copy /Y "%%f" "%EXE_DIR%\" >nul 2>&1 && echo   %%~nxf
    )
)
if exist "%QT_ROOT%\bin\icuin*.dll" (
    for %%f in ("%QT_ROOT%\bin\icuin*.dll") do (
        copy /Y "%%f" "%EXE_DIR%\" >nul 2>&1 && echo   %%~nxf
    )
)

REM === COPY PLATFORM PLUGIN ===
set PLATFORM_DIR=%EXE_DIR%\platforms
if not exist "%PLATFORM_DIR%" mkdir "%PLATFORM_DIR%"
copy /Y "%QT_ROOT%\plugins\platforms\qwindows.dll" "%PLATFORM_DIR%\" >nul 2>&1 && echo   platforms\qwindows.dll

REM === COPY STYLE PLUGINS ===
set STYLES_DIR=%EXE_DIR%\styles
if not exist "%STYLES_DIR%" mkdir "%STYLES_DIR%"
if exist "%QT_ROOT%\plugins\styles\*.dll" (
    copy /Y "%QT_ROOT%\plugins\styles\*.dll" "%STYLES_DIR%\" >nul 2>&1 && echo   styles\*.dll
)

REM === COPY IMAGEFORMAT PLUGINS ===
set IMAGEFORMAT_DIR=%EXE_DIR%\imageformats
if not exist "%IMAGEFORMAT_DIR%" mkdir "%IMAGEFORMAT_DIR%"
if exist "%QT_ROOT%\plugins\imageformats\*.dll" (
    copy /Y "%QT_ROOT%\plugins\imageformats\*.dll" "%IMAGEFORMAT_DIR%\" >nul 2>&1
)

REM === COPY VC++ RUNTIME DLLS (if needed) ===
REM These are usually in the Qt bin directory or system32
if exist "%QT_ROOT%\bin\msvcp140.dll" (
    copy /Y "%QT_ROOT%\bin\msvcp140.dll" "%EXE_DIR%\" >nul 2>&1
)
if exist "%QT_ROOT%\bin\vcruntime140.dll" (
    copy /Y "%QT_ROOT%\bin\vcruntime140.dll" "%EXE_DIR%\" >nul 2>&1
)
if exist "%QT_ROOT%\bin\vcruntime140_1.dll" (
    copy /Y "%QT_ROOT%\bin\vcruntime140_1.dll" "%EXE_DIR%\" >nul 2>&1
)

echo.
echo ============================================
echo ✅ Qt DLLs deployed successfully!
echo ============================================
echo.
echo The executable should now run correctly.
echo Location: %EXE_PATH%
echo.

:end

