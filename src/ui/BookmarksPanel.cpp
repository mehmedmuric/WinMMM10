#include "BookmarksPanel.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

BookmarksPanel::BookmarksPanel(QWidget* parent)
    : QDockWidget("Bookmarks", parent)
{
    setupUI();
}

void BookmarksPanel::setupUI() {
    QWidget* widget = new QWidget(this);
    setWidget(widget);
    auto* layout = new QVBoxLayout(widget);
    
    // Filter
    auto* filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Category:", this));
    m_categoryFilter = new QComboBox(this);
    m_categoryFilter->addItem("All", "");
    m_categoryFilter->setEditable(false);
    connect(m_categoryFilter, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this, &BookmarksPanel::onCategoryFilterChanged);
    filterLayout->addWidget(m_categoryFilter);
    layout->addLayout(filterLayout);
    
    // Bookmark tree
    m_bookmarkTree = new QTreeWidget(this);
    m_bookmarkTree->setHeaderLabels({"Name", "Address", "Category", "Description"});
    m_bookmarkTree->setRootIsDecorated(false);
    m_bookmarkTree->setAlternatingRowColors(true);
    m_bookmarkTree->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_bookmarkTree, &QTreeWidget::itemSelectionChanged, this, &BookmarksPanel::onBookmarkSelected);
    connect(m_bookmarkTree, &QTreeWidget::itemDoubleClicked, this, &BookmarksPanel::onBookmarkDoubleClicked);
    layout->addWidget(m_bookmarkTree);
    
    // Add bookmark section
    auto* addGroup = new QGroupBox("Add Bookmark", this);
    auto* addLayout = new QVBoxLayout(addGroup);
    
    auto* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Name:", this));
    m_nameEdit = new QLineEdit(this);
    nameLayout->addWidget(m_nameEdit);
    addLayout->addLayout(nameLayout);
    
    auto* addrLayout = new QHBoxLayout();
    addrLayout->addWidget(new QLabel("Address:", this));
    m_addressEdit = new QLineEdit(this);
    m_addressEdit->setPlaceholderText("0x00000000");
    addrLayout->addWidget(m_addressEdit);
    addLayout->addLayout(addrLayout);
    
    auto* catLayout = new QHBoxLayout();
    catLayout->addWidget(new QLabel("Category:", this));
    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->setEditable(true);
    m_categoryCombo->addItems({"General", "Maps", "Checksums", "Data"});
    catLayout->addWidget(m_categoryCombo);
    addLayout->addLayout(catLayout);
    
    m_descriptionEdit = new QLineEdit(this);
    m_descriptionEdit->setPlaceholderText("Description (optional)");
    addLayout->addWidget(m_descriptionEdit);
    
    m_addButton = new QPushButton("Add Bookmark", this);
    connect(m_addButton, &QPushButton::clicked, this, &BookmarksPanel::onAddBookmark);
    addLayout->addWidget(m_addButton);
    
    layout->addWidget(addGroup);
    
    // Delete button
    m_deleteButton = new QPushButton("Delete Selected", this);
    m_deleteButton->setEnabled(false);
    connect(m_deleteButton, &QPushButton::clicked, this, &BookmarksPanel::onDeleteBookmark);
    layout->addWidget(m_deleteButton);
}

void BookmarksPanel::setBookmarkManager(BookmarkManager* manager) {
    m_bookmarkManager = manager;
    refreshBookmarks();
}

void BookmarksPanel::refreshBookmarks() {
    populateBookmarks();
}

void BookmarksPanel::populateBookmarks() {
    if (!m_bookmarkManager) {
        m_bookmarkTree->clear();
        return;
    }
    
    m_bookmarkTree->clear();
    
    QString filterCategory = m_categoryFilter->currentData().toString();
    std::vector<Bookmark> bookmarks;
    if (filterCategory.isEmpty()) {
        bookmarks = m_bookmarkManager->getAllBookmarks();
    } else {
        bookmarks = m_bookmarkManager->getBookmarksByCategory(filterCategory.toStdString());
    }
    
    for (const auto& bookmark : bookmarks) {
        auto* item = new QTreeWidgetItem(m_bookmarkTree);
        item->setText(0, QString::fromStdString(bookmark.name));
        
        std::ostringstream oss;
        oss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << bookmark.address;
        item->setText(1, QString::fromStdString(oss.str()));
        item->setData(1, Qt::UserRole, static_cast<qulonglong>(bookmark.address));
        
        item->setText(2, QString::fromStdString(bookmark.category));
        item->setText(3, QString::fromStdString(bookmark.description));
    }
    
    m_bookmarkTree->resizeColumnToContents(0);
    m_bookmarkTree->resizeColumnToContents(1);
    
    // Update category filter
    m_categoryFilter->clear();
    m_categoryFilter->addItem("All", "");
    for (const auto& cat : m_bookmarkManager->getCategories()) {
        m_categoryFilter->addItem(QString::fromStdString(cat), QString::fromStdString(cat));
    }
}

void BookmarksPanel::onAddBookmark() {
    if (!m_bookmarkManager) return;
    
    QString name = m_nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Invalid Bookmark", "Bookmark name cannot be empty.");
        return;
    }
    
    QString addrText = m_addressEdit->text().trimmed();
    bool ok;
    size_t address = 0;
    if (addrText.startsWith("0x") || addrText.startsWith("0X")) {
        address = addrText.toULongLong(&ok, 16);
    } else {
        address = addrText.toULongLong(&ok, 16);
        if (!ok) {
            address = addrText.toULongLong(&ok, 10);
        }
    }
    
    if (!ok) {
        QMessageBox::warning(this, "Invalid Address", "Please enter a valid address.");
        return;
    }
    
    Bookmark bookmark;
    bookmark.name = name.toStdString();
    bookmark.address = address;
    bookmark.category = m_categoryCombo->currentText().toStdString();
    bookmark.description = m_descriptionEdit->text().toStdString();
    
    m_bookmarkManager->addBookmark(bookmark);
    refreshBookmarks();
    
    m_nameEdit->clear();
    m_addressEdit->clear();
    m_descriptionEdit->clear();
}

void BookmarksPanel::onDeleteBookmark() {
    auto* item = m_bookmarkTree->currentItem();
    if (!item || !m_bookmarkManager) return;
    
    size_t address = item->data(1, Qt::UserRole).toULongLong();
    m_bookmarkManager->removeBookmark(address);
    refreshBookmarks();
}

void BookmarksPanel::onBookmarkSelected() {
    auto* item = m_bookmarkTree->currentItem();
    m_deleteButton->setEnabled(item != nullptr);
    if (item) {
        size_t address = item->data(1, Qt::UserRole).toULongLong();
        emit bookmarkSelected(address);
    }
}

void BookmarksPanel::onBookmarkDoubleClicked(QTreeWidgetItem* item, int column) {
    Q_UNUSED(column);
    if (item) {
        size_t address = item->data(1, Qt::UserRole).toULongLong();
        emit bookmarkDoubleClicked(address);
    }
}

void BookmarksPanel::onCategoryFilterChanged(const QString& category) {
    Q_UNUSED(category);
    populateBookmarks();
}

void BookmarksPanel::goToBookmark(size_t address) {
    for (int i = 0; i < m_bookmarkTree->topLevelItemCount(); ++i) {
        auto* item = m_bookmarkTree->topLevelItem(i);
        if (item->data(1, Qt::UserRole).toULongLong() == address) {
            m_bookmarkTree->setCurrentItem(item);
            m_bookmarkTree->scrollToItem(item);
            break;
        }
    }
}

} // namespace WinMMM10

