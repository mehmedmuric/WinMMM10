# Building WinMMM10 Editor

## Prerequisites

- CMake 3.20 or higher
- Qt 6.x (Core, Gui, Widgets, OpenGL, OpenGLWidgets, Json)
- C++20 compatible compiler
  - Windows: MSVC 2019 or later (recommended: Visual Studio 2022)
  - Linux: GCC 10+ or Clang 12+
  - macOS: Xcode 12+

## Building on Windows

### Using Visual Studio

1. Open "x64 Native Tools Command Prompt for VS 2022"
2. Navigate to the project directory
3. Run:
   ```batch
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Release
   ```

### Using Qt Creator

1. Open Qt Creator
2. File -> Open File or Project
3. Select CMakeLists.txt
4. Configure the project
5. Build -> Build Project

## Building on Linux

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## Building on macOS

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

## Creating Installer (Windows)

### Using Inno Setup

1. Install Inno Setup Compiler
2. Open `installer/WinMMM10Editor.iss`
3. Build -> Compile

Note: Update the Qt6 paths in the .iss file to match your Qt installation.

### Manual Distribution

Copy the following to a distribution folder:
- WinMMM10Editor.exe
- All Qt DLLs from Qt6/bin
- Qt plugins (platforms, styles, etc.)

## Troubleshooting

### Qt Not Found

Set CMAKE_PREFIX_PATH:
```bash
cmake .. -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2019_64
```

### Missing DLLs on Windows

Use Qt's windeployqt tool:
```batch
windeployqt.exe WinMMM10Editor.exe
```

### OpenGL Issues

Ensure your graphics drivers are up to date and OpenGL 2.0+ is supported.

