# Preparing WinMMM10 Editor for Distribution

## Quick Steps

### 1. Clean Build
```cmd
build_release.bat
```

### 2. Create Installer
```cmd
create_installer.bat
```

The installer will be created at: `dist\WinMMM10Editor_Setup.exe`

## What's Included in the Installer

The installer automatically includes:
- WinMMM10Editor.exe (main executable)
- All required Qt6 DLLs (Core, Gui, Widgets, OpenGL, OpenGLWidgets, Svg, Network)
- ICU DLLs (icuuc, icudt, icuin) for internationalization
- Qt platform plugins (qwindows.dll)
- Qt style plugins
- Qt image format plugins (GIF, JPEG, SVG, ICO)
- Qt icon engines (SVG icon support)
- Qt network and TLS plugins

## System Requirements

- Windows 10 or later (64-bit)
- Visual C++ Redistributable 2015-2022 (x64)
  - Usually installed automatically by Windows Update
  - If missing, download from: https://aka.ms/vs/17/release/vc_redist.x64.exe

## Distribution Checklist

- [ ] Build completed successfully
- [ ] Installer created without errors
- [ ] Test installer on clean Windows machine
- [ ] Verify all DLLs are included
- [ ] Test application startup
- [ ] Verify all features work correctly

## Installation Instructions for End Users

1. Run `WinMMM10Editor_Setup.exe`
2. Follow the installation wizard
3. Launch WinMMM10 Editor from Start Menu or desktop shortcut
4. If you see DLL errors, install Visual C++ Redistributable (link above)

## Troubleshooting

### Missing DLL Errors
- Install Visual C++ Redistributable 2015-2022 (x64)
- Ensure all Qt DLLs are in the installation directory

### Application Won't Start
- Check Windows Event Viewer for error details
- Verify Visual C++ Redistributable is installed
- Check that all plugin directories exist (platforms, styles, imageformats, etc.)

### Stack Overflow on Startup
- This should be fixed in the current version
- If it occurs, check the log file: `WinMMM10Editor.log` in the installation directory
