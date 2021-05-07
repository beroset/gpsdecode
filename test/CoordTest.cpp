#include <iostream>
#include <iomanip>
#include <future>
#include <string>
#include <sstream>
#include "WANAddr.h"
#include <gtest/gtest.h>

using namespace amrnet;

TEST(CoordTest, createCoord) {
    Coord antarctica1{88, 56, 47, 450, false};
    ASSERT_EQ(antarctica1, Coord{-88.9465142});
}

TEST(CoordTest, CompareGTEpsilon) {
    // minimum discernable difference is about 0.0015 arcseconds
    // the distance below is detectable
    Coord antarctica1{88, 56, 47, 450, false};
    Coord antarctica2{88, 56, 47, 452, false};
    ASSERT_NE(antarctica1, antarctica2);
}

TEST(CoordTest, CompareLTEpsilon) {
    // minimum discernable difference is about 0.0015 arcseconds
    // the distance below is NOT detectable
    Coord antarctica1{88, 56, 47, 451, false};
    Coord antarctica2{88, 56, 47, 452, false};
    ASSERT_EQ(antarctica1, antarctica2);
}

TEST(CoordTest, createWANAddr) {
    WANAddr addr{ 0xFE, 0x80, 0x70, 0x0e, 0x8E, 0x05};
    ASSERT_EQ(addr.getColor(), 5);
}

TEST(CoordTest, getLatFromWANAddr) {
    WANAddr addr{ 0xFE, 0x80, 0x70, 0x0e, 0x8E, 0x05};
    ASSERT_EQ(addr.getLat(), -88.9465142);
}
    
TEST(CoordTest, getLongFromWANAddr) {
    WANAddr addr{ 0xFE, 0x80, 0x70, 0x0e, 0x8E, 0x05};
    ASSERT_EQ(addr.getLong(), 2.55844111);
}

TEST(CoordTest, getDMSFromWANAddr) {
    WANAddr addr{ 0xFE, 0x80, 0x70, 0x0e, 0x8E, 0x05};
    DMS dms{addr};
    ASSERT_EQ(dms.getLat(), -88.9465142);
    ASSERT_EQ(dms.getLong(), 2.55844111);
    ASSERT_EQ(dms.getColor(), 5);
}

TEST(CoordTest, getWANAddrFromDMS) {
    WANAddr addr{ 0xFE, 0x80, 0x70, 0x0e, 0x8E, 0x05};
    DMS dms{-88.9465142, 2.55844111, 5};
    ASSERT_EQ(dms.asWANAddr(), addr);
}

TEST(CoordTest, printWANAddr) {
    std::stringstream ss;
    WANAddr addr{ 0x4F, 0x93, 0x1E, 0x20, 0x9d, 0xe0};
    ss << addr;
    std::string s{ss.str()};
    ASSERT_EQ(s, "4f:93:1e:20:9d:e0");
}

TEST(CoordTest, printNWDMS) {
    std::stringstream ss;
    WANAddr addr{ 0x4F, 0x93, 0x1E, 0x20, 0x9d, 0xe0};
    ss << DMS{addr};
    std::string s{ss.str()};
    // note that \xC2\xB0 is the degree symbol in UTF8
    ASSERT_EQ(s, "34\xC2\xB0" "2'56.831\"N, 84\xC2\xB0" "15'59.745\"W C 0");
}

TEST(CoordTest, printSEDMS) {
    DMS dms{{ 88, 56, 47, 451, false}, { 10, 42, 37,  68, true }, 30};
    std::stringstream ss;
    ss << dms;
    std::string s{ss.str()};
    // note that \xC2\xB0 is the degree symbol in UTF8
    ASSERT_EQ(s, "88\xC2\xB0" "56'47.450\"S, 10\xC2\xB0" "42'37.067\"E C 30");
}

TEST(CoordTest, compareDMSfromWANAddr) {
    std::stringstream ss;
    WANAddr addr1{ 0x4f, 0x93, 0x1e, 0x20, 0x9d, 0xe0};
    // color change
    WANAddr addr2{ 0x4f, 0x93, 0x1e, 0x20, 0x9d, 0xe0^0x01};
    // lsb of longitude change
    WANAddr addr3{ 0x4f, 0x93, 0x1e, 0x20, 0x9d, 0xe0^0x20};
    // sign of longitude change
    WANAddr addr4{ 0x4f, 0x93, 0x1e^0x04, 0x20, 0x9d, 0xe0};
    // lsb of latitude change
    WANAddr addr5{ 0x4f, 0x93, 0x1e^0x08, 0x20, 0x9d, 0xe0};
    // sign of latitude change
    WANAddr addr6{ 0x4f^0x80, 0x93, 0x1e, 0x20, 0x9d, 0xe0};
    DMS dms1{addr1};
    DMS dms2{addr2};
    DMS dms3{addr3};
    DMS dms4{addr4};
    DMS dms5{addr5};
    DMS dms6{addr6};
    ASSERT_NE(dms1, dms2);
    ASSERT_NE(dms1, dms3);
    ASSERT_NE(dms1, dms4);
    ASSERT_NE(dms1, dms5);
    ASSERT_NE(dms1, dms6);
}
