#pragma once

#include <cstdint>
#include <vector>
#include <stack>
#include <memory>

namespace WinMMM10 {

struct Edit {
    size_t offset;
    uint8_t oldValue;
    uint8_t newValue;
};

class EditHistory {
public:
    EditHistory();
    ~EditHistory() = default;
    
    void pushEdit(size_t offset, uint8_t oldValue, uint8_t newValue);
    bool canUndo() const { return !m_undoStack.empty(); }
    bool canRedo() const { return !m_redoStack.empty(); }
    
    Edit undo();
    Edit redo();
    
    void clear();
    void clearRedo();
    
    size_t undoCount() const { return m_undoStack.size(); }
    size_t redoCount() const { return m_redoStack.size(); }

private:
    std::stack<Edit> m_undoStack;
    std::stack<Edit> m_redoStack;
};

} // namespace WinMMM10

