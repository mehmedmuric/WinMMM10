#include "SearchReplaceDialog.h"
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QRegExpValidator>

namespace WinMMM10 {

SearchReplaceDialog::SearchReplaceDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Search and Replace");
    setModal(false);
    resize(500, 300);
    
    setupUI();
}

void SearchReplaceDialog::setupUI() {
    auto* layout = new QVBoxLayout(this);
    
    // Search section
    auto* searchGroup = new QGroupBox("Search", this);
    auto* searchLayout = new QVBoxLayout(searchGroup);
    
    auto* searchTextLayout = new QHBoxLayout();
    searchTextLayout->addWidget(new QLabel("Find:", this));
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Enter search text or hex pattern");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &SearchReplaceDialog::onSearchTextChanged);
    searchTextLayout->addWidget(m_searchEdit);
    searchLayout->addLayout(searchTextLayout);
    
    // Search mode
    auto* modeGroup = new QButtonGroup(this);
    m_hexRadio = new QRadioButton("Hex", this);
    m_textRadio = new QRadioButton("Text", this);
    m_patternRadio = new QRadioButton("Pattern", this);
    m_hexRadio->setChecked(true);
    
    modeGroup->addButton(m_hexRadio, 0);
    modeGroup->addButton(m_textRadio, 1);
    modeGroup->addButton(m_patternRadio, 2);
    
    auto* modeLayout = new QHBoxLayout();
    modeLayout->addWidget(m_hexRadio);
    modeLayout->addWidget(m_textRadio);
    modeLayout->addWidget(m_patternRadio);
    modeLayout->addStretch();
    searchLayout->addLayout(modeLayout);
    
    // Options
    m_caseSensitiveCheck = new QCheckBox("Case sensitive", this);
    m_wrapAroundCheck = new QCheckBox("Wrap around", this);
    m_wrapAroundCheck->setChecked(true);
    searchLayout->addWidget(m_caseSensitiveCheck);
    searchLayout->addWidget(m_wrapAroundCheck);
    
    layout->addWidget(searchGroup);
    
    // Replace section
    auto* replaceGroup = new QGroupBox("Replace", this);
    auto* replaceLayout = new QVBoxLayout(replaceGroup);
    
    auto* replaceTextLayout = new QHBoxLayout();
    replaceTextLayout->addWidget(new QLabel("Replace with:", this));
    m_replaceEdit = new QLineEdit(this);
    m_replaceEdit->setPlaceholderText("Enter replacement text or hex");
    replaceTextLayout->addWidget(m_replaceEdit);
    replaceLayout->addLayout(replaceTextLayout);
    
    layout->addWidget(replaceGroup);
    
    // Buttons
    auto* buttonLayout = new QHBoxLayout();
    m_findNextButton = new QPushButton("Find Next", this);
    m_findNextButton->setDefault(true);
    m_findPreviousButton = new QPushButton("Find Previous", this);
    m_replaceButton = new QPushButton("Replace", this);
    m_replaceAllButton = new QPushButton("Replace All", this);
    
    connect(m_findNextButton, &QPushButton::clicked, this, &SearchReplaceDialog::onFindNext);
    connect(m_findPreviousButton, &QPushButton::clicked, this, &SearchReplaceDialog::onFindPrevious);
    connect(m_replaceButton, &QPushButton::clicked, this, &SearchReplaceDialog::onReplace);
    connect(m_replaceAllButton, &QPushButton::clicked, this, &SearchReplaceDialog::onReplaceAll);
    
    buttonLayout->addWidget(m_findNextButton);
    buttonLayout->addWidget(m_findPreviousButton);
    buttonLayout->addWidget(m_replaceButton);
    buttonLayout->addWidget(m_replaceAllButton);
    buttonLayout->addStretch();
    
    layout->addLayout(buttonLayout);
    
    // Status label
    m_statusLabel = new QLabel("", this);
    layout->addWidget(m_statusLabel);
    
    // Dialog buttons
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    onSearchTextChanged();
}

QString SearchReplaceDialog::searchText() const {
    return m_searchEdit->text();
}

QString SearchReplaceDialog::replaceText() const {
    return m_replaceEdit->text();
}

SearchReplaceDialog::SearchMode SearchReplaceDialog::searchMode() const {
    if (m_hexRadio->isChecked()) return Hex;
    if (m_textRadio->isChecked()) return Text;
    return Pattern;
}

bool SearchReplaceDialog::caseSensitive() const {
    return m_caseSensitiveCheck->isChecked();
}

bool SearchReplaceDialog::searchForward() const {
    return true; // Always forward for find next
}

bool SearchReplaceDialog::wrapAround() const {
    return m_wrapAroundCheck->isChecked();
}

void SearchReplaceDialog::onFindNext() {
    if (m_searchEdit->text().isEmpty()) {
        m_statusLabel->setText("Search text is empty");
        return;
    }
    emit findNext();
}

void SearchReplaceDialog::onFindPrevious() {
    if (m_searchEdit->text().isEmpty()) {
        m_statusLabel->setText("Search text is empty");
        return;
    }
    emit findPrevious();
}

void SearchReplaceDialog::onReplace() {
    if (m_searchEdit->text().isEmpty()) {
        m_statusLabel->setText("Search text is empty");
        return;
    }
    emit replace();
}

void SearchReplaceDialog::onReplaceAll() {
    if (m_searchEdit->text().isEmpty()) {
        m_statusLabel->setText("Search text is empty");
        return;
    }
    
    int ret = QMessageBox::question(this, "Replace All",
                                   "This will replace all occurrences. Continue?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        emit replaceAll();
    }
}

void SearchReplaceDialog::onSearchTextChanged() {
    bool hasText = !m_searchEdit->text().isEmpty();
    m_findNextButton->setEnabled(hasText);
    m_findPreviousButton->setEnabled(hasText);
    m_replaceButton->setEnabled(hasText);
    m_replaceAllButton->setEnabled(hasText);
    
    if (!hasText) {
        m_statusLabel->setText("");
    }
}

} // namespace WinMMM10

