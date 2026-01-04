#pragma once

#include <QtTest/QtTest>
#include "../src/mappacks/MapPack.h"

class TestMapPack : public QObject {
    Q_OBJECT

private slots:
    void testCreateMapPack();
    void testSaveLoadMapPack();
};

