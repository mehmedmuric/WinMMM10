#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace WinMMM10 {

struct Bookmark {
    std::string name;
    size_t address;
    std::string category;
    std::string description;
    int64_t timestamp{0};
    
    Bookmark() = default;
    Bookmark(const std::string& n, size_t addr, const std::string& cat = "", const std::string& desc = "")
        : name(n), address(addr), category(cat), description(desc) {}
};

class BookmarkManager {
public:
    BookmarkManager() = default;
    ~BookmarkManager() = default;
    
    void addBookmark(const Bookmark& bookmark);
    void removeBookmark(size_t address);
    void removeBookmark(const std::string& name);
    Bookmark* findBookmark(size_t address);
    Bookmark* findBookmark(const std::string& name);
    const Bookmark* findBookmark(size_t address) const;
    const Bookmark* findBookmark(const std::string& name) const;
    
    std::vector<Bookmark> getAllBookmarks() const;
    std::vector<Bookmark> getBookmarksByCategory(const std::string& category) const;
    std::vector<std::string> getCategories() const;
    
    bool hasBookmark(size_t address) const;
    void clear();
    size_t count() const { return m_bookmarks.size(); }

private:
    std::map<size_t, Bookmark> m_bookmarks; // Keyed by address for fast lookup
    std::map<std::string, size_t> m_nameIndex; // Name to address mapping
};

} // namespace WinMMM10

