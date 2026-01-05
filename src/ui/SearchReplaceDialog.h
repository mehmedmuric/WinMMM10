#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QGroupBox>

namespace WinMMM10 {

class SearchReplaceDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchReplaceDialog(QWidget* parent = nullptr);
    ~SearchReplaceDialog() override = default;
    
    enum SearchMode {
        Hex,
        Text,
        Pattern
    };
    
    QString searchText() const;
    QString replaceText() const;
    SearchMode searchMode() const;
    bool caseSensitive() const;
    bool searchForward() const;
    bool wrapAround() const;

signals:
    void findNext();
    void findPrevious();
    void replace();
    void replaceAll();

private slots:
    void onFindNext();
    void onFindPrevious();
    void onReplace();
    void onReplaceAll();
    void onSearchTextChanged();

private:
    void setupUI();
    
    QLineEdit* m_searchEdit{nullptr};
    QLineEdit* m_replaceEdit{nullptr};
    QRadioButton* m_hexRadio{nullptr};
    QRadioButton* m_textRadio{nullptr};
    QRadioButton* m_patternRadio{nullptr};
    QCheckBox* m_caseSensitiveCheck{nullptr};
    QCheckBox* m_wrapAroundCheck{nullptr};
    QPushButton* m_findNextButton{nullptr};
    QPushButton* m_findPreviousButton{nullptr};
    QPushButton* m_replaceButton{nullptr};
    QPushButton* m_replaceAllButton{nullptr};
    QLabel* m_statusLabel{nullptr};
};

} // namespace WinMMM10

