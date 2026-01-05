# Preparing WinMMM10 Editor for Distribution

This guide walks through the complete process of preparing WinMMM10 Editor for distribution to other devices.

## Quick Checklist

- [ ] Clean all build caches
- [ ] Build release version
- [ ] Test the executable
- [ ] Configure installer script
- [ ] Create installer package
- [ ] Test installer on clean system
- [ ] Package for distribution

## Step-by-Step Process

### 1. Clean Build Environment

Run the cleanup script to remove all cached files and build artifacts:

```batch
clean_cache.bat
```

This removes:
- Build output directories (`out`, `build`)
- CMake cache files
- Visual Studio cache (`.vs`)
- User-specific files (`.user`, `.suo`, etc.)

### 2. Build Release Version

Build the application in Release mode:

```batch
build_release.bat
```

**Important:** If your Qt installation is not in the default location, set the `QT_ROOT` environment variable first:

```batch
set QT_ROOT=C:\Qt\6.10.1\msvc2022_64
build_release.bat
```

The executable will be created at:
```
out\build\x64-Release\Release\WinMMM10Editor.exe
```

### 3. Test the Executable

Before creating the installer, test that the executable works:

1. Navigate to: `out\build\x64-Release\Release\`
2. Double-click `WinMMM10Editor.exe`
3. Verify the application launches correctly
4. Test basic functionality

### 4. Configure Installer Script

Edit `installer\WinMMM10Editor.iss` and update the Qt path:

```inno
#define QtRoot "C:\Qt\6.10.1\msvc2022_64"
```

**Important:** This path must match your Qt installation. The installer will bundle DLLs from this location.

### 5. Create Installer Package

Run the installer creation script:

```batch
create_installer.bat
```

This will:
- Verify Inno Setup is installed
- Compile the installer script
- Create `dist\WinMMM10Editor_Setup.exe`

**Note:** If Inno Setup is not installed:
- Download from: https://jrsoftware.org/isdl.php
- Or update the path in `create_installer.bat`

### 6. Test the Installer

**Critical Step:** Test the installer on a clean system (or VM):

1. Copy `dist\WinMMM10Editor_Setup.exe` to a test machine
2. Run the installer
3. Verify installation completes successfully
4. Launch the application
5. Test core functionality
6. Verify all Qt DLLs are present in installation directory
7. Test uninstallation

### 7. Package for Distribution

Once tested, the installer is ready for distribution:

**Distribution Methods:**
- **USB Drive:** Copy `WinMMM10Editor_Setup.exe` to USB
- **Network Share:** Place on shared drive
- **Cloud Storage:** Upload to OneDrive, Google Drive, etc.
- **Website:** Host for download
- **Email:** Send as attachment (if size allows)

**Include Documentation:**
- `DISTRIBUTION_README.md` - For end users
- `INSTALLATION_GUIDE.md` - Detailed installation instructions
- `README.md` - Application documentation

## File Locations

### Build Output
```
out\build\x64-Release\Release\WinMMM10Editor.exe
```

### Installer Output
```
dist\WinMMM10Editor_Setup.exe
```

### Source Files
```
installer\WinMMM10Editor.iss  (Installer script)
```

## Troubleshooting

### Build Fails

**Qt Not Found:**
- Set `QT_ROOT` environment variable
- Or update default path in `build_release.bat`

**Visual Studio Tools Missing:**
- Use "x64 Native Tools Command Prompt for VS 2022"
- Ensure "Desktop development with C++" workload is installed

### Installer Creation Fails

**Inno Setup Not Found:**
- Install Inno Setup from https://jrsoftware.org/isdl.php
- Or update path in `create_installer.bat`

**Qt DLLs Not Found:**
- Verify Qt path in `installer\WinMMM10Editor.iss`
- Ensure Qt is installed at specified path
- Check DLL files exist in `{QtRoot}\bin\`

### Application Won't Run After Installation

**Missing DLLs:**
- Check installer script includes all required Qt DLLs
- Use `windeployqt` to identify missing dependencies:
  ```batch
  windeployqt.exe out\build\x64-Release\Release\WinMMM10Editor.exe
  ```

**Visual C++ Runtime Missing:**
- Users need Microsoft Visual C++ Redistributable
- Download: https://aka.ms/vs/17/release/vc_redist.x64.exe
- Consider bundling with installer (advanced)

## Distribution Checklist

Before distributing:

- [ ] Application builds successfully
- [ ] Executable runs and functions correctly
- [ ] Installer script configured with correct Qt path
- [ ] Installer creates successfully
- [ ] Installer tested on clean Windows system
- [ ] Application launches after installation
- [ ] All Qt DLLs present in installation directory
- [ ] No missing dependency errors
- [ ] Uninstaller works correctly
- [ ] Version number correct in installer
- [ ] Documentation included (optional but recommended)

## System Requirements for Target Machines

- **OS:** Windows 10 (64-bit) or later
- **RAM:** 4 GB minimum, 8 GB recommended
- **Disk Space:** 150 MB
- **Graphics:** OpenGL 2.0+ compatible
- **Runtime:** Microsoft Visual C++ Redistributable 2015-2022 (may need separate installation)

## Additional Notes

- The installer requires administrator privileges
- Users can choose custom installation location
- All Qt dependencies are bundled - no separate Qt installation needed
- Application does not require internet connection
- Consider code signing for production releases (reduces Windows SmartScreen warnings)

## Next Steps After Distribution

1. Monitor for installation issues
2. Collect user feedback
3. Update documentation based on common questions
4. Plan for future updates and versioning

