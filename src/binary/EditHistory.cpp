#include "EditHistory.h"

namespace WinMMM10 {

EditHistory::EditHistory() = default;

void EditHistory::pushEdit(size_t offset, uint8_t oldValue, uint8_t newValue) {
    if (oldValue == newValue) {
        return;
    }
    
    m_undoStack.push({offset, oldValue, newValue});
    clearRedo();
}

Edit EditHistory::undo() {
    if (m_undoStack.empty()) {
        return {0, 0, 0};
    }
    
    Edit edit = m_undoStack.top();
    m_undoStack.pop();
    m_redoStack.push({edit.offset, edit.newValue, edit.oldValue});
    return edit;
}

Edit EditHistory::redo() {
    if (m_redoStack.empty()) {
        return {0, 0, 0};
    }
    
    Edit edit = m_redoStack.top();
    m_redoStack.pop();
    m_undoStack.push({edit.offset, edit.newValue, edit.oldValue});
    return edit;
}

void EditHistory::clear() {
    while (!m_undoStack.empty()) {
        m_undoStack.pop();
    }
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }
}

void EditHistory::clearRedo() {
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }
}

} // namespace WinMMM10

