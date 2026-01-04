#include "TestMapDetection.h"
#include <QTest>
#include "../src/binary/Endianness.h"

void TestMapDetection::testDetect2DMap() {
    // Create test data with a simple 2D map pattern
    std::vector<uint8_t> data(256, 0);
    
    // Fill with a simple increasing pattern
    for (size_t i = 0; i < 32; ++i) {
        uint16_t value = static_cast<uint16_t>(i * 100);
        data[i * 2] = value & 0xFF;
        data[i * 2 + 1] = (value >> 8) & 0xFF;
    }
    
    WinMMM10::MapDetector detector;
    detector.setBinaryData(data.data(), data.size());
    
    auto candidates = detector.detectMaps(0, data.size());
    QVERIFY(!candidates.empty());
}

void TestMapDetection::testDetect3DMap() {
    // Create test data with a 3D map pattern (16x16)
    std::vector<uint8_t> data(1024, 0);
    
    // Fill with a pattern
    for (size_t row = 0; row < 16; ++row) {
        for (size_t col = 0; col < 16; ++col) {
            size_t index = (row * 16 + col) * 2;
            uint16_t value = static_cast<uint16_t>((row * 100) + col);
            data[index] = value & 0xFF;
            data[index + 1] = (value >> 8) & 0xFF;
        }
    }
    
    WinMMM10::MapDetector detector;
    detector.setBinaryData(data.data(), data.size());
    
    auto candidates = detector.detectMaps(0, data.size());
    QVERIFY(!candidates.empty());
}
