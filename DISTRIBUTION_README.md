# WinMMM10 Editor - Distribution Package

## Quick Start for Distribution

This package contains everything needed to install and run WinMMM10 Editor on Windows systems.

### For End Users

1. **Run the Installer**
   - Double-click `WinMMM10Editor_Setup.exe`
   - Follow the installation wizard
   - The installer will automatically install all required components

2. **Launch the Application**
   - Find "WinMMM10 Editor" in the Start Menu
   - Or use the desktop shortcut (if created during installation)

3. **System Requirements**
   - Windows 10 (64-bit) or later
   - 4 GB RAM minimum
   - 150 MB free disk space

### For Developers/IT Administrators

#### Installation Location
- Default: `C:\Program Files\WinMMM10Editor`
- Custom location can be chosen during installation

#### Included Components
- WinMMM10Editor.exe (main application)
- Qt6 Core Libraries (Qt6Core.dll, Qt6Gui.dll, Qt6Widgets.dll)
- Qt6 OpenGL Libraries (Qt6OpenGL.dll, Qt6OpenGLWidgets.dll)
- Qt6 Platform Plugins (Windows platform support)
- Qt6 Style Plugins (optional)

#### Silent Installation
To install silently (for IT deployment):
```batch
WinMMM10Editor_Setup.exe /SILENT /DIR="C:\Program Files\WinMMM10Editor"
```

#### Uninstallation
- Use Windows "Add or Remove Programs"
- Or run: `C:\Program Files\WinMMM10Editor\unins000.exe`

## Troubleshooting

### Application Won't Start

**Error: "The program can't start because MSVCP140.dll is missing"**
- Install Microsoft Visual C++ Redistributable 2015-2022
- Download: https://aka.ms/vs/17/release/vc_redist.x64.exe

**Error: "Qt6Core.dll not found"**
- Reinstall the application
- Ensure all files were copied during installation
- Check Windows Defender or antivirus didn't quarantine files

**Application Crashes on Startup**
- Check Windows Event Viewer for error details
- Ensure graphics drivers are up to date
- Try running as Administrator

### Missing Features

If certain features don't work:
- Ensure all Qt DLLs are present in the installation directory
- Check that the `platforms` subdirectory contains `qwindows.dll`
- Verify OpenGL support (required for 3D map viewing)

## File Verification

After installation, verify these files exist:
```
C:\Program Files\WinMMM10Editor\
├── WinMMM10Editor.exe
├── Qt6Core.dll
├── Qt6Gui.dll
├── Qt6Widgets.dll
├── Qt6OpenGL.dll
├── Qt6OpenGLWidgets.dll
├── platforms\
│   └── qwindows.dll
└── unins000.exe (uninstaller)
```

## Support

For issues or questions:
- Check the main README.md for application documentation
- Review INSTALLATION_GUIDE.md for detailed installation instructions
- Check TROUBLESHOOTING_BUILD.md for build-related issues

## Version Information

- Application Version: 1.0.0
- Build Date: See installer properties
- Architecture: x64 (64-bit Windows only)

## License

See LICENSE.txt for license information.

