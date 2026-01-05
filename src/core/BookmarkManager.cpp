#include "BookmarkManager.h"
#include <algorithm>
#include <set>

namespace WinMMM10 {

void BookmarkManager::addBookmark(const Bookmark& bookmark) {
    // Remove old bookmark at this address if exists
    auto it = m_bookmarks.find(bookmark.address);
    if (it != m_bookmarks.end()) {
        m_nameIndex.erase(it->second.name);
    }
    
    // Remove old bookmark with same name if exists
    auto nameIt = m_nameIndex.find(bookmark.name);
    if (nameIt != m_nameIndex.end() && nameIt->second != bookmark.address) {
        m_bookmarks.erase(nameIt->second);
    }
    
    m_bookmarks[bookmark.address] = bookmark;
    m_nameIndex[bookmark.name] = bookmark.address;
}

void BookmarkManager::removeBookmark(size_t address) {
    auto it = m_bookmarks.find(address);
    if (it != m_bookmarks.end()) {
        m_nameIndex.erase(it->second.name);
        m_bookmarks.erase(it);
    }
}

void BookmarkManager::removeBookmark(const std::string& name) {
    auto nameIt = m_nameIndex.find(name);
    if (nameIt != m_nameIndex.end()) {
        m_bookmarks.erase(nameIt->second);
        m_nameIndex.erase(nameIt);
    }
}

Bookmark* BookmarkManager::findBookmark(size_t address) {
    auto it = m_bookmarks.find(address);
    if (it != m_bookmarks.end()) {
        return &it->second;
    }
    return nullptr;
}

Bookmark* BookmarkManager::findBookmark(const std::string& name) {
    auto nameIt = m_nameIndex.find(name);
    if (nameIt != m_nameIndex.end()) {
        auto it = m_bookmarks.find(nameIt->second);
        if (it != m_bookmarks.end()) {
            return &it->second;
        }
    }
    return nullptr;
}

const Bookmark* BookmarkManager::findBookmark(size_t address) const {
    auto it = m_bookmarks.find(address);
    if (it != m_bookmarks.end()) {
        return &it->second;
    }
    return nullptr;
}

const Bookmark* BookmarkManager::findBookmark(const std::string& name) const {
    auto nameIt = m_nameIndex.find(name);
    if (nameIt != m_nameIndex.end()) {
        auto it = m_bookmarks.find(nameIt->second);
        if (it != m_bookmarks.end()) {
            return &it->second;
        }
    }
    return nullptr;
}

std::vector<Bookmark> BookmarkManager::getAllBookmarks() const {
    std::vector<Bookmark> result;
    result.reserve(m_bookmarks.size());
    for (const auto& [addr, bookmark] : m_bookmarks) {
        result.push_back(bookmark);
    }
    return result;
}

std::vector<Bookmark> BookmarkManager::getBookmarksByCategory(const std::string& category) const {
    std::vector<Bookmark> result;
    for (const auto& [addr, bookmark] : m_bookmarks) {
        if (bookmark.category == category) {
            result.push_back(bookmark);
        }
    }
    return result;
}

std::vector<std::string> BookmarkManager::getCategories() const {
    std::set<std::string> categories;
    for (const auto& [addr, bookmark] : m_bookmarks) {
        if (!bookmark.category.empty()) {
            categories.insert(bookmark.category);
        }
    }
    return std::vector<std::string>(categories.begin(), categories.end());
}

bool BookmarkManager::hasBookmark(size_t address) const {
    return m_bookmarks.find(address) != m_bookmarks.end();
}

void BookmarkManager::clear() {
    m_bookmarks.clear();
    m_nameIndex.clear();
}

} // namespace WinMMM10

