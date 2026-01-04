#include <QtTest/QtTest>
#include "TestChecksum.h"
#include "TestMapDetection.h"
#include "TestMapPack.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    
    int result = 0;
    
    TestChecksum testChecksum;
    result |= QTest::qExec(&testChecksum, argc, argv);
    
    TestMapDetection testMapDetection;
    result |= QTest::qExec(&testMapDetection, argc, argv);
    
    TestMapPack testMapPack;
    result |= QTest::qExec(&testMapPack, argc, argv);
    
    return result;
}

