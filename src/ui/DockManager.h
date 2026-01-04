#pragma once

#include <QObject>

namespace WinMMM10 {

// Placeholder for future dock manager functionality
class DockManager : public QObject {
    Q_OBJECT

public:
    explicit DockManager(QObject* parent = nullptr) : QObject(parent) {}
    ~DockManager() override = default;
};

} // namespace WinMMM10

