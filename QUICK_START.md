# Quick Start Guide - Building and Distribution

## Important: Prerequisites Required

Before building, you need:

1. **CMake 3.20+** - Download from: https://cmake.org/download/
2. **Qt 6.x** - Download from: https://www.qt.io/download
3. **Visual Studio 2022** (with C++ desktop development workload)
4. **Inno Setup** (for creating installer) - Download from: https://jrsoftware.org/isdl.php

## Step 1: Build the Application

### Option A: Using Visual Studio Developer Command Prompt (Recommended)

1. Open "x64 Native Tools Command Prompt for VS 2022"
2. Navigate to project folder:
   ```
   cd C:\Users\mehme\OneDrive\Desktop\WinMMM10
   ```
3. Run the build script:
   ```
   build_release.bat
   ```

### Option B: Manual Build

Open "x64 Native Tools Command Prompt for VS 2022" and run:

```batch
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cd ..
```

The executable will be at: `build\Release\WinMMM10Editor.exe`

## Step 2: Create Installer

After building, you have two options:

### Option A: Using the Script

1. Make sure Inno Setup is installed
2. Run:
   ```
   create_installer.bat
   ```

**NOTE:** You may need to edit `installer\WinMMM10Editor.iss` and update the Qt paths (lines 28-30) to match your Qt installation directory.

### Option B: Manual Installer Creation

1. Install Inno Setup Compiler
2. Open `installer\WinMMM10Editor.iss` in Inno Setup
3. Update Qt paths in the script (lines 28-30)
4. Build → Compile (F9)

The installer will be created at: `dist\WinMMM10Editor_Setup.exe`

## Step 3: Copy to USB Flash Drive

Simply copy `dist\WinMMM10Editor_Setup.exe` to your USB drive!

On other computers, run the installer to install WinMMM10 Editor.

## Troubleshooting

### CMake not found
- Install CMake and add it to PATH, OR
- Use Visual Studio Developer Command Prompt (has tools in PATH)

### Qt not found during build
- Set CMAKE_PREFIX_PATH in build script:
  ```
  cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2019_64
  ```

### Installer creation fails
- Check that build was successful first
- Update Qt paths in `installer\WinMMM10Editor.iss`
- Make sure Inno Setup is installed

### Missing DLLs when running
- Use Qt's windeployqt tool after building:
  ```
  cd build\Release
  C:\Qt\6.x.x\msvc2019_64\bin\windeployqt.exe WinMMM10Editor.exe
  ```

