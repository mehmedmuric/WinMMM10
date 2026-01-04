# Build Troubleshooting Guide

## Error: "The build tools for Visual Studio 2022 (Platform Toolset = 'v143') cannot be found"

### Cause
Visual Studio 2022 is installed, but the C++ build tools are missing.

### Solution 1: Install C++ Build Tools (Recommended)

1. Open **Visual Studio Installer**
   - Press Windows Key, type "Visual Studio Installer"
   - Or find it in Start Menu

2. Find **Visual Studio 2022** in the list
   - Click the **"Modify"** button

3. Select **"Desktop development with C++"** workload
   - Check the box if not already checked
   - This installs the v143 toolset

4. Click **"Modify"** button (bottom right)
   - Wait for installation to complete (may take 10-30 minutes)

5. Close Visual Studio Installer
   - Restart your command prompt
   - Run `build_release.bat` again

### Solution 2: Use Visual Studio 2019 Toolset

If you have Visual Studio 2019 installed:

1. The updated `build_release.bat` will automatically try v142 toolset
2. Or use `build_release_vs2019.bat` script (uses VS 2019 directly)

### Solution 3: Use Visual Studio 2019 Generator

Run manually:
```batch
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

## Other Common Issues

### CMake Not Found
- Use "x64 Native Tools Command Prompt for VS 2022" (has CMake in PATH)
- Or install CMake and add to PATH

### Qt Not Found
Add Qt path to CMake command:
```batch
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2019_64
```

### Build Errors
- Check error messages for missing files or syntax errors
- Ensure all source files are present
- Try cleaning: `rmdir /s /q build` then rebuild

## Quick Reference

**Main build script:** `build_release.bat`
- Tries VS 2022 with v143, then v142, then VS 2019
- Use this first

**VS 2019 script:** `build_release_vs2019.bat`
- Uses Visual Studio 2019 directly
- Use if you only have VS 2019

**Manual build:**
```batch
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -T v143
cmake --build . --config Release
```

