#include "TestChecksum.h"
#include <QTest>

void TestChecksum::testSimpleSum() {

void TestChecksum::testSimpleSum() {
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint32_t result = WinMMM10::ChecksumManager::instance().calculateChecksum(
        WinMMM10::ChecksumType::SimpleSum, data, 5);
    QCOMPARE(result, static_cast<uint32_t>(0x0F)); // 1+2+3+4+5 = 15 = 0x0F
}

void TestChecksum::testCRC16() {
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    uint32_t result = WinMMM10::ChecksumManager::instance().calculateChecksum(
        WinMMM10::ChecksumType::CRC16, data, 9);
    QVERIFY(result != 0);
}

void TestChecksum::testCRC32() {
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    uint32_t result = WinMMM10::ChecksumManager::instance().calculateChecksum(
        WinMMM10::ChecksumType::CRC32, data, 9);
    QVERIFY(result != 0);
}

void TestChecksum::testXOR() {
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    uint32_t result = WinMMM10::ChecksumManager::instance().calculateChecksum(
        WinMMM10::ChecksumType::XOR, data, 4);
    QCOMPARE(result, static_cast<uint32_t>(0x04)); // 1^2^3^4 = 4
}

void TestChecksum::testVerifyChecksum() {
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x0F}; // Last byte is checksum
    bool valid = WinMMM10::ChecksumManager::instance().verifyChecksum(
        WinMMM10::ChecksumType::SimpleSum, data, 5, 4, 0, 4);
    QVERIFY(valid);
}

