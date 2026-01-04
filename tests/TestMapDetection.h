#pragma once

#include <QtTest/QtTest>
#include "../src/heuristics/MapDetector.h"

class TestMapDetection : public QObject {
    Q_OBJECT

private slots:
    void testDetect2DMap();
    void testDetect3DMap();
};

