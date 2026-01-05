#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace WinMMM10 {

struct Annotation {
    size_t address;
    std::string note;
    std::string color; // Hex color code
    int64_t timestamp{0};
    
    Annotation() = default;
    Annotation(size_t addr, const std::string& n, const std::string& col = "#FFFF00")
        : address(addr), note(n), color(col) {}
};

class AnnotationManager {
public:
    AnnotationManager() = default;
    ~AnnotationManager() = default;
    
    void addAnnotation(const Annotation& annotation);
    void removeAnnotation(size_t address);
    Annotation* findAnnotation(size_t address);
    const Annotation* findAnnotation(size_t address) const;
    
    std::vector<Annotation> getAllAnnotations() const;
    std::vector<Annotation> getAnnotationsInRange(size_t startAddress, size_t endAddress) const;
    
    bool hasAnnotation(size_t address) const;
    void clear();
    size_t count() const { return m_annotations.size(); }

private:
    std::map<size_t, Annotation> m_annotations; // Keyed by address
};

} // namespace WinMMM10

