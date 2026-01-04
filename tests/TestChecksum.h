#pragma once

#include <QtTest/QtTest>
#include "../src/binary/Checksum.h"

class TestChecksum : public QObject {
    Q_OBJECT

private slots:
    void testSimpleSum();
    void testCRC16();
    void testCRC32();
    void testXOR();
    void testVerifyChecksum();
};

