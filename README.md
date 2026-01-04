# WinMMM10 Editor

Professional ECU binary map editor for automotive tuning and research.

## Features

- **Binary File Handling**: Load and edit large binary files (up to 16MB+)
- **HEX Editor**: Full-featured hex editor with undo/redo
- **Automatic Map Detection**: Enhanced heuristic-based detection with confidence scoring
- **Map Visualization**: 2D curve and 3D surface viewers
- **Project System**: Save/load projects with map definitions
- **Map Packs**: Import/export collections of map definitions
- **Checksum Algorithms**: Multiple algorithms (CRC-16/32, XOR, Sum, Additive)
- **Plugin System**: Extensible architecture for custom functionality
- **Testing Framework**: Comprehensive unit tests for stability
- **Documentation**: Complete workflow and API documentation

## Building

### Requirements

- CMake 3.20 or higher
- Qt 6.x (Core, Gui, Widgets, OpenGL, OpenGLWidgets, Json, Test)
- C++20 compatible compiler (MSVC 2019+, GCC 10+, Clang 12+)

### Windows (MSVC)

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Running Tests

```bash
cd build
ctest --config Release
# Or run directly:
Release\WinMMM10Editor_Tests.exe
```

### Linux

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## Installation

Run the installer `WinMMM10Editor_Setup.exe` and follow the installation wizard.

## Usage

1. Create a new project or open an existing one
2. Load a binary file (.bin)
3. Use automatic map detection or define maps manually
4. Edit maps using the 2D/3D viewers
5. Verify checksums before and after modifications
6. Export map packs for sharing
7. Validate changes and export the modified binary

## Documentation

See the `docs/` directory for:
- **WORKFLOW.md**: Complete workflow guide
- **API.md**: API reference documentation
- **PLUGINS.md**: Plugin development guide
- **BUILD.md**: Detailed build instructions

## Features in Detail

### Checksum Algorithms

Supports multiple checksum algorithms commonly used in ECU binaries:
- Simple Sum (8-bit, 16-bit)
- CRC-16, CRC-32
- XOR (8-bit, 16-bit)
- Additive (8-bit, 16-bit)

### Automatic Map Detection

Enhanced detection engine with:
- Two-pass scanning for better accuracy
- Confidence scoring
- Overlap detection and filtering
- Support for 2D and 3D maps

### Map Packs

Import and export collections of map definitions:
- Share map definitions with team
- Include ECU identification info
- Version control for definitions

### Plugin System

Extensible plugin architecture:
- Custom map detection algorithms
- Custom checksum calculations
- Data validation functions
- Easy to extend with new functionality

## License

Proprietary - For internal use only
