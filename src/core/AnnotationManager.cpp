#include "AnnotationManager.h"
#include <algorithm>

namespace WinMMM10 {

void AnnotationManager::addAnnotation(const Annotation& annotation) {
    m_annotations[annotation.address] = annotation;
}

void AnnotationManager::removeAnnotation(size_t address) {
    m_annotations.erase(address);
}

Annotation* AnnotationManager::findAnnotation(size_t address) {
    auto it = m_annotations.find(address);
    if (it != m_annotations.end()) {
        return &it->second;
    }
    return nullptr;
}

const Annotation* AnnotationManager::findAnnotation(size_t address) const {
    auto it = m_annotations.find(address);
    if (it != m_annotations.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<Annotation> AnnotationManager::getAllAnnotations() const {
    std::vector<Annotation> result;
    result.reserve(m_annotations.size());
    for (const auto& [addr, annotation] : m_annotations) {
        result.push_back(annotation);
    }
    return result;
}

std::vector<Annotation> AnnotationManager::getAnnotationsInRange(size_t startAddress, size_t endAddress) const {
    std::vector<Annotation> result;
    for (const auto& [addr, annotation] : m_annotations) {
        if (addr >= startAddress && addr <= endAddress) {
            result.push_back(annotation);
        }
    }
    return result;
}

bool AnnotationManager::hasAnnotation(size_t address) const {
    return m_annotations.find(address) != m_annotations.end();
}

void AnnotationManager::clear() {
    m_annotations.clear();
}

} // namespace WinMMM10

