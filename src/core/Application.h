#pragma once

namespace WinMMM10 {

class Application {
public:
    Application() = default;
    ~Application() = default;
    
    static const char* applicationName() { return "WinMMM10 Editor"; }
    static const char* organizationName() { return "WinMMM10"; }
    static const char* version() { return "1.0.0"; }
};

} // namespace WinMMM10

