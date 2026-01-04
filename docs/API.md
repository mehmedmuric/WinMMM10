# WinMMM10 Editor - API Documentation

## Core Classes

### BinaryFile

Handles binary file loading, saving, and data access.

```cpp
bool load(const std::string& filepath);
bool save(const std::string& filepath);
uint8_t readByte(size_t offset) const;
bool writeByte(size_t offset, uint8_t value);
// ... more read/write methods
```

### MapDefinition

Represents a map definition with address, dimensions, and scaling.

```cpp
void setName(const std::string& name);
void setAddress(size_t address);
void setType(MapType type);
void setRows(size_t rows);
void setColumns(size_t columns);
void setDataType(uint16_t type);
void setFactor(double factor);
void setOffset(double offset);
```

### ChecksumManager

Manages checksum calculation and verification.

```cpp
uint32_t calculateChecksum(ChecksumType type, const uint8_t* data, size_t size,
                          size_t startOffset = 0, size_t endOffset = 0);
bool verifyChecksum(ChecksumType type, const uint8_t* data, size_t size,
                   size_t checksumOffset, size_t startOffset = 0, size_t endOffset = 0);
```

### MapDetector

Automatic map detection engine.

```cpp
void setBinaryData(const uint8_t* data, size_t size);
std::vector<MapCandidate> detectMaps(size_t minAddress = 0, size_t maxAddress = 0);
```

### MapPack

Map pack management for importing/exporting map collections.

```cpp
bool saveToFile(const std::string& filepath) const;
bool loadFromFile(const std::string& filepath);
void addMap(const MapDefinition& map);
```

### PluginInterface

Interface for creating plugins.

```cpp
virtual std::string name() const = 0;
virtual std::string version() const = 0;
virtual bool initialize() = 0;
virtual void shutdown() = 0;
virtual std::vector<size_t> detectMaps(const uint8_t* data, size_t size);
virtual uint32_t calculateChecksum(const uint8_t* data, size_t size);
```

## Plugin Development

### Creating a Plugin

1. Create a class implementing `PluginInterface`
2. Use Q_DECLARE_INTERFACE macro
3. Export using Q_PLUGIN_METADATA

Example:

```cpp
class MyPlugin : public QObject, public WinMMM10::PluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_INTERFACE_IID)
    Q_INTERFACES(WinMMM10::PluginInterface)

public:
    std::string name() const override { return "My Plugin"; }
    std::string version() const override { return "1.0.0"; }
    bool initialize() override { return true; }
    void shutdown() override {}
};
```

### Plugin Capabilities

- Custom map detection algorithms
- Custom checksum calculations
- Data validation
- Extended file format support
- Custom UI components (future)

## Checksum Algorithms

Supported algorithms:
- Simple Sum (8-bit, 16-bit)
- CRC-16, CRC-32
- XOR (8-bit, 16-bit)
- Additive (8-bit, 16-bit)

## Map Detection

The detection engine uses:
- Pattern analysis (monotonicity, variance)
- Matrix pattern recognition
- Confidence scoring
- Overlap detection and filtering

