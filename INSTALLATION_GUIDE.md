# Installation Guide for WinMMM10 Editor

This guide explains how to install and distribute WinMMM10 Editor on other devices.

## For End Users (Installing on Other Devices)

### Prerequisites

- Windows 10 or later (64-bit)
- No additional software required - all dependencies are included in the installer

### Installation Steps

1. **Obtain the Installer**
   - Copy `WinMMM10Editor_Setup.exe` from the `dist` folder to a USB drive or network share
   - Or download it from the distribution location

2. **Run the Installer**
   - Double-click `WinMMM10Editor_Setup.exe`
   - If Windows SmartScreen appears, click "More info" then "Run anyway" (the app is not code-signed)
   - Follow the installation wizard
   - Choose installation location (default: `C:\Program Files\WinMMM10Editor`)
   - Optionally create desktop shortcut

3. **Launch the Application**
   - From Start Menu: Search for "WinMMM10 Editor"
   - From Desktop: Double-click the shortcut (if created)
   - From Installation Directory: Navigate to installation folder and run `WinMMM10Editor.exe`

### Uninstallation

1. Open "Add or Remove Programs" (Settings > Apps)
2. Find "WinMMM10 Editor"
3. Click "Uninstall"
4. Follow the uninstaller prompts

## For Developers (Creating the Installer)

### Step 1: Clean Build Environment

```batch
clean_cache.bat
```

This removes all build artifacts and cache files for a fresh build.

### Step 2: Build the Application

```batch
build_release.bat
```

This will:
- Configure CMake
- Build the Release version
- Create executable at: `out\build\x64-Release\Release\WinMMM10Editor.exe`

**Note:** Make sure to set `QT_ROOT` environment variable if your Qt installation is not in the default location:
```batch
set QT_ROOT=C:\Qt\6.10.1\msvc2022_64
build_release.bat
```

### Step 3: Configure Installer Script

Edit `installer\WinMMM10Editor.iss` and update the Qt path:

```inno
#define QtRoot "C:\Qt\6.10.1\msvc2022_64"
```

Update this to match your Qt installation path. The installer will bundle the required Qt DLLs.

### Step 4: Create the Installer

```batch
create_installer.bat
```

This will:
- Check for Inno Setup Compiler
- Compile the installer script
- Create `dist\WinMMM10Editor_Setup.exe`

### Step 5: Test the Installer

1. Run the installer on a test machine (or VM)
2. Verify the application launches correctly
3. Test basic functionality
4. Check that all Qt DLLs are present in the installation directory

### Step 6: Distribute

Copy `dist\WinMMM10Editor_Setup.exe` to:
- USB drive
- Network share
- Distribution website
- Cloud storage

## Troubleshooting

### Application Won't Start

**Missing DLL Error:**
- The installer may be missing some Qt DLLs
- Check `installer\WinMMM10Editor.iss` and ensure all required DLLs are listed
- You can use Qt's `windeployqt` tool to identify missing dependencies:
  ```batch
  windeployqt.exe out\build\x64-Release\Release\WinMMM10Editor.exe
  ```

**Visual C++ Runtime Error:**
- Install Microsoft Visual C++ Redistributable
- Download from: https://aka.ms/vs/17/release/vc_redist.x64.exe

### Installer Creation Fails

**Inno Setup Not Found:**
- Install Inno Setup from: https://jrsoftware.org/isdl.php
- Or update the path in `create_installer.bat`

**Qt DLLs Not Found:**
- Verify Qt path in `installer\WinMMM10Editor.iss` is correct
- Ensure Qt is installed at the specified path
- Check that the DLL files exist in `{QtRoot}\bin\`

### Build Fails

**Qt Not Found:**
- Set `QT_ROOT` environment variable before building
- Or update the default path in `build_release.bat`

**Visual Studio Tools Missing:**
- Use "x64 Native Tools Command Prompt for VS 2022" (or appropriate version)
- Ensure "Desktop development with C++" workload is installed

## File Structure

After installation, the application directory contains:

```
WinMMM10Editor/
├── WinMMM10Editor.exe    (Main executable)
├── Qt6Core.dll           (Qt Core library)
├── Qt6Gui.dll            (Qt GUI library)
├── Qt6Widgets.dll        (Qt Widgets library)
├── Qt6OpenGL.dll         (Qt OpenGL library)
├── Qt6OpenGLWidgets.dll  (Qt OpenGL Widgets library)
├── platforms/
│   └── qwindows.dll      (Windows platform plugin)
└── styles/               (Qt style plugins, if any)
```

## System Requirements

- **OS:** Windows 10 (64-bit) or later
- **RAM:** 4 GB minimum, 8 GB recommended
- **Disk Space:** 100 MB for application + 50 MB for Qt libraries
- **Graphics:** OpenGL 2.0+ compatible graphics card

## Distribution Checklist

Before distributing the installer:

- [ ] Application builds successfully
- [ ] Installer creates successfully
- [ ] Installer tested on clean Windows system
- [ ] Application launches and runs correctly
- [ ] All Qt DLLs are included
- [ ] No missing dependency errors
- [ ] Uninstaller works correctly
- [ ] Version number is correct in installer

## Notes

- The installer requires administrator privileges to install to Program Files
- Users can choose a different installation location during setup
- The application does not require internet connection to run
- All Qt dependencies are bundled - no separate Qt installation needed on target machines

