# Build Status and Instructions

## What Has Been Created

I've created the following files to help you build and create the installer:

1. **build_release.bat** - Automated build script
2. **create_installer.bat** - Automated installer creation script
3. **QUICK_START.md** - Quick reference guide
4. **BUILD_INSTRUCTIONS.txt** - Detailed instructions

## Important: You Need to Build From Visual Studio Command Prompt

The build requires Visual Studio tools which are automatically available in the Developer Command Prompt.

## How to Build (Step by Step)

### Step 1: Open the Right Command Prompt

**DO NOT use regular PowerShell or CMD!**

Instead:
1. Press Windows Key
2. Type: "Developer Command Prompt for VS 2022"
3. Select: **"x64 Native Tools Command Prompt for VS 2022"**
4. This opens a special command prompt with all build tools in PATH

### Step 2: Navigate to Project Folder

In the Developer Command Prompt, type:
```
cd C:\Users\mehme\OneDrive\Desktop\WinMMM10
```

### Step 3: Run the Build Script

```
build_release.bat
```

This will:
- Configure CMake
- Build the Release version
- Create `build\Release\WinMMM10Editor.exe`

### Step 4: Create the Installer

After build succeeds, run:
```
create_installer.bat
```

**NOTE:** You may need to:
1. Install Inno Setup first (if not already installed)
2. Edit `installer\WinMMM10Editor.iss` and update Qt paths (lines 28-30)

The installer will be created at: `dist\WinMMM10Editor_Setup.exe`

### Step 5: Copy to USB

Copy `dist\WinMMM10Editor_Setup.exe` to your USB flash drive!

## Prerequisites Checklist

Before building, make sure you have:

- [ ] CMake 3.20+ installed (usually auto-detected in VS Command Prompt)
- [ ] Qt 6.x installed (Core, Gui, Widgets, OpenGL, OpenGLWidgets, Json, Test)
- [ ] Visual Studio 2022 with C++ Desktop Development workload
- [ ] Inno Setup (for creating installer) - optional but recommended

## What to Do Now

1. Open "x64 Native Tools Command Prompt for VS 2022"
2. Navigate to: `C:\Users\mehme\OneDrive\Desktop\WinMMM10`
3. Run: `build_release.bat`
4. Wait for build to complete
5. Run: `create_installer.bat`
6. Copy `dist\WinMMM10Editor_Setup.exe` to USB

## Troubleshooting

**"CMake not found"**
→ Use Visual Studio Developer Command Prompt (not regular CMD/PowerShell)

**"Qt not found"**
→ You may need to set CMAKE_PREFIX_PATH. Edit build_release.bat and add:
   `-DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2019_64` to the cmake command

**"Build failed"**
→ Check error messages. Common issues:
   - Missing Qt components
   - Missing Visual Studio C++ tools
   - Syntax errors in code (check error output)

**"Installer creation failed"**
→ Make sure:
   - Build was successful
   - Inno Setup is installed
   - Qt paths in installer\WinMMM10Editor.iss are correct

