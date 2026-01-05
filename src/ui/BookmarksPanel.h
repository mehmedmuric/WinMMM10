#pragma once

#include <QDockWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../core/BookmarkManager.h"

namespace WinMMM10 {

class BookmarksPanel : public QDockWidget {
    Q_OBJECT

public:
    explicit BookmarksPanel(QWidget* parent = nullptr);
    ~BookmarksPanel() override = default;
    
    void setBookmarkManager(BookmarkManager* manager);
    void refreshBookmarks();
    void goToBookmark(size_t address);

signals:
    void bookmarkSelected(size_t address);
    void bookmarkDoubleClicked(size_t address);
    void bookmarkAddRequested(size_t address);
    void bookmarkDeleteRequested(size_t address);

private slots:
    void onAddBookmark();
    void onDeleteBookmark();
    void onBookmarkSelected();
    void onBookmarkDoubleClicked(QTreeWidgetItem* item, int column);
    void onCategoryFilterChanged(const QString& category);

private:
    void setupUI();
    void populateBookmarks();
    
    BookmarkManager* m_bookmarkManager{nullptr};
    QTreeWidget* m_bookmarkTree{nullptr};
    QLineEdit* m_nameEdit{nullptr};
    QLineEdit* m_addressEdit{nullptr};
    QComboBox* m_categoryCombo{nullptr};
    QLineEdit* m_descriptionEdit{nullptr};
    QPushButton* m_addButton{nullptr};
    QPushButton* m_deleteButton{nullptr};
    QComboBox* m_categoryFilter{nullptr};
};

} // namespace WinMMM10

