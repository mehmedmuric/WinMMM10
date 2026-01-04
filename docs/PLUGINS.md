# Plugin Development Guide

## Overview

WinMMM10 Editor supports plugins for extending functionality. Plugins can provide custom map detection, checksum algorithms, and validation functions.

## Plugin Structure

### Basic Plugin Template

```cpp
#include "plugins/PluginInterface.h"
#include <QObject>

class MyPlugin : public QObject, public WinMMM10::PluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_INTERFACE_IID)
    Q_INTERFACES(WinMMM10::PluginInterface)

public:
    std::string name() const override {
        return "My Plugin";
    }
    
    std::string version() const override {
        return "1.0.0";
    }
    
    std::string author() const override {
        return "Your Name";
    }
    
    std::string description() const override {
        return "Plugin description";
    }
    
    bool initialize() override {
        // Initialize plugin
        return true;
    }
    
    void shutdown() override {
        // Cleanup
    }
};

#include "MyPlugin.moc"
```

### Custom Map Detection

Override `detectMaps()` to provide custom detection:

```cpp
std::vector<size_t> detectMaps(const uint8_t* data, size_t size) override {
    std::vector<size_t> addresses;
    // Your detection logic
    return addresses;
}
```

### Custom Checksum

Override `calculateChecksum()`:

```cpp
uint32_t calculateChecksum(const uint8_t* data, size_t size) override {
    // Your checksum algorithm
    return checksum;
}
```

## Building Plugins

1. Create a CMakeLists.txt for your plugin
2. Link against Qt6 and WinMMM10 headers
3. Build as shared library (.dll/.so)
4. Place in `plugins/` directory

## Plugin Loading

Plugins are automatically loaded from the `plugins/` directory on startup. Use `PluginManager::instance()` to access plugins programmatically.

## Best Practices

- Handle errors gracefully
- Don't modify binary data directly
- Return empty vectors/null values on failure
- Document your plugin's functionality
- Test thoroughly before distribution

