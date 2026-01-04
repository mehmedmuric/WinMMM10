#include "TestMapPack.h"
#include <QTest>
#include <QTemporaryFile>
#include <QDir>
#include <QFile>

void TestMapPack::testCreateMapPack() {
    WinMMM10::MapPack pack;
    pack.info().name = "Test Pack";
    pack.info().version = "1.0.0";
    
    WinMMM10::MapDefinition map;
    map.setName("Test Map");
    map.setAddress(0x1000);
    map.setType(WinMMM10::MapType::Map2D);
    map.setColumns(16);
    
    pack.addMap(map);
    QCOMPARE(pack.mapCount(), static_cast<size_t>(1));
}

void TestMapPack::testSaveLoadMapPack() {
    WinMMM10::MapPack pack;
    pack.info().name = "Test Pack";
    pack.info().version = "1.0.0";
    pack.info().author = "Test Author";
    
    WinMMM10::MapDefinition map;
    map.setName("Test Map");
    map.setAddress(0x1000);
    pack.addMap(map);
    
    QTemporaryFile tempFile;
    tempFile.setAutoRemove(false);
    QVERIFY(tempFile.open());
    QString filepath = tempFile.fileName();
    tempFile.close();
    
    QVERIFY(pack.saveToFile(filepath.toStdString()));
    
    WinMMM10::MapPack loadedPack;
    QVERIFY(loadedPack.loadFromFile(filepath.toStdString()));
    QCOMPARE(loadedPack.info().name, std::string("Test Pack"));
    QCOMPARE(loadedPack.mapCount(), pack.mapCount());
    
    QFile::remove(filepath);
}
