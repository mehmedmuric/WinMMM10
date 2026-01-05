#pragma once

#include <QObject>
#include <string>
#include <vector>
#include <cstdint>

namespace WinMMM10 {

class PluginInterface : public QObject {
    Q_OBJECT

public:
    explicit PluginInterface(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~PluginInterface() = default;
    
    virtual std::string name() const = 0;
    virtual std::string version() const = 0;
    virtual std::string author() const = 0;
    virtual std::string description() const = 0;
    
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    // Optional: Custom map detection
    virtual std::vector<size_t> detectMaps(const uint8_t* data, size_t size) { 
        Q_UNUSED(data);
        Q_UNUSED(size);
        return {}; 
    }
    
    // Optional: Custom checksum calculation
    virtual uint32_t calculateChecksum(const uint8_t* data, size_t size) { 
        Q_UNUSED(data);
        Q_UNUSED(size);
        return 0; 
    }
    
    // Optional: Custom validation
    virtual bool validateData(const uint8_t* data, size_t size) { 
        Q_UNUSED(data);
        Q_UNUSED(size);
        return true; 
    }
};

#define PLUGIN_INTERFACE_IID "com.winmmm10.PluginInterface/1.0"
Q_DECLARE_INTERFACE(PluginInterface, PLUGIN_INTERFACE_IID)

} // namespace WinMMM10

