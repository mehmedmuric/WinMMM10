# WinMMM10 Editor - Workflow Guide

## Overview

WinMMM10 Editor is a professional ECU binary map editor for automotive tuning and research. This guide outlines the typical workflow for using the application.

## Basic Workflow

### 1. Creating a New Project

1. **File → New Project**
   - Enter project name
   - Enter ECU name/identifier
   - Optionally add description
   - Specify binary file path
   - Click OK and save the project file (.wmm10)

### 2. Loading a Binary File

1. **File → Load Binary** (or specify during project creation)
   - Select your ECU binary file (.bin)
   - The file will be loaded into memory
   - HEX editor will display the binary content

### 3. Automatic Map Detection

1. **Map → Detect Maps**
   - The system will scan the binary for potential maps
   - Review detected candidates with confidence scores
   - Select maps to add to your project
   - Maps are added with default scaling (you can edit later)

### 4. Manual Map Definition

1. **Map → Add Map**
   - Enter map name
   - Specify address (hex format: 0x1234)
   - Select map type (2D or 3D)
   - Set rows and columns
   - Configure data type (uint8, uint16, int16, float)
   - Set scaling factor and offset
   - Configure axis definitions (X and Y for 3D maps)
   - Add unit labels

### 5. Viewing and Editing Maps

1. **Select a map** from the Maps dock panel
   - 2D maps: View as curve in 2D Viewer tab
   - 3D maps: View as surface in 3D Viewer tab (rotate with mouse, zoom with wheel)

2. **Editing**:
   - Use HEX editor to modify raw values
   - Map viewers update in real-time
   - Changes are tracked for undo/redo

### 6. Working with Map Packs

**Exporting Map Packs:**
1. Define all maps in your project
2. File → Export Map Pack
3. Fill in pack information (name, version, author, ECU info)
4. Save as .mappack file

**Importing Map Packs:**
1. File → Import Map Pack
2. Select .mappack file
3. Maps will be added to current project
4. Verify addresses match your binary

### 7. Checksum Verification

1. **Tools → Calculate Checksum**
   - Select checksum algorithm
   - Specify address range
   - View calculated checksum
   - Compare with stored checksum

2. **Tools → Verify Checksum**
   - Select algorithm and checksum location
   - System verifies and reports status

### 8. Saving Changes

1. **Save Binary**: File → Save Binary (Ctrl+Alt+S)
   - Saves modified binary to original location
   - Use "Save Binary As" to create backup

2. **Save Project**: File → Save Project (Ctrl+S)
   - Saves map definitions and project metadata
   - Project file contains all map information

## Advanced Workflow

### Using Plugins

1. **Install Plugins**:
   - Place plugin DLL files in `plugins/` directory
   - Plugins are auto-loaded on startup
   - View loaded plugins in Help → About → Plugins

2. **Custom Algorithms**:
   - Plugins can provide custom map detection
   - Custom checksum algorithms
   - Validation functions

### Validation and Safety

1. **Before Making Changes**:
   - Always create a backup of original binary
   - Use validation tools to check value ranges
   - Verify checksums before and after changes

2. **Validation Settings**:
   - Set maximum change percentage
   - Define value ranges
   - Enable warnings for extreme values

### Best Practices

1. **Project Organization**:
   - Use descriptive map names
   - Add notes/descriptions
   - Group related maps

2. **Map Definitions**:
   - Verify scaling factors with known values
   - Test with small changes first
   - Document axis units and meanings

3. **Collaboration**:
   - Share map packs instead of project files
   - Include ECU identification info
   - Document custom algorithms used

## Keyboard Shortcuts

- **Ctrl+N**: New Project
- **Ctrl+O**: Open Project
- **Ctrl+S**: Save Project
- **Ctrl+Alt+S**: Save Binary
- **Ctrl+Z**: Undo
- **Ctrl+Shift+Z**: Redo
- **Ctrl+F**: Find/Go To Address
- **F5**: Refresh/Reload
- **F11**: Toggle Fullscreen

## Troubleshooting

### Maps Not Detected
- Try manual detection with different parameters
- Check if binary format matches expected endianness
- Verify binary file is complete and not corrupted

### Checksum Verification Fails
- Verify correct algorithm is selected
- Check checksum location/offset
- Some ECUs use multiple checksums

### Performance Issues
- Close unused map viewers
- Reduce HEX editor view range for large files
- Disable real-time validation for faster editing

## Tips

- Use map packs to share definitions with team
- Export/import definitions as JSON for version control
- Keep original binaries in separate backup folder
- Document all custom scaling factors and offsets
- Test changes on test ECUs before production use

