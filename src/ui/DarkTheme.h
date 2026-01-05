#pragma once

#include <QString>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>

namespace WinMMM10 {

class DarkTheme {
public:
    static void apply(QApplication* app);
};

} // namespace WinMMM10

