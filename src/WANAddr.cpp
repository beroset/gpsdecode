#include "WANAddr.h"
#include <iomanip>
#include <cmath>

namespace amrnet {
Coord::Coord(uint8_t degrees, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, bool NE)
    : a{(NE ? 1.0 : -1.0) * (
    (milliseconds / 3600000.0) +
    (seconds / 3600.0) + 
    (minutes /60.0) + 
    degrees
    )}
{}

Coord::Coord(double a) 
    : a{a}
{}

std::ostream& operator<<(std::ostream& out, const Coord& dms) {
    double a = dms.toDouble();
    bool NE{!(a < 0)};
    if (!NE) {
        a = std::abs(a);
    }
    unsigned degrees = a;
    a = (a - degrees) * 60;
    unsigned minutes = a;
    a = (a - minutes) * 60;
    unsigned seconds = a;
    unsigned milliseconds = (a - seconds) * 1000;
    auto savedflags = out.setf(std::ios::dec);
    out << degrees << "°" 
            << minutes << '\'' 
            << seconds << '.' 
            << std::setfill('0') << std::setw(3) << milliseconds << '"';
    out.setf(savedflags);
    return out;
}

Coord WANAddr::getLat() const {
    uint32_t mylat{0};
    auto count{sizeof(mylat)};
    for (auto it{begin()}; count; --count) {
        mylat = (mylat << 8) | *it++;
    } 
    // we only need 20 bits of the 32
    mylat >>= 11;
    // remember the sign bit
    bool South{(mylat & 0x10'0000) != 0};
    // and zero the sign bit
    mylat &= 0xfffff;
    return Coord{(double)(mylat * DMS::invanglefactor + (South ? 0 : 90))};
}

Coord WANAddr::getLong() const {
    uint32_t mylong{0};
    auto count{sizeof(mylong)};
    for (auto it{begin() + 2}; count; --count) {
        mylong = (mylong << 8) | *it++;
    } 
    // we only need 21 bits of the 32
    mylong >>= 5;
    // remember the sign bit
    bool West{(mylong & 0x20'0000) != 0};
    // and zero the sign bit
    mylong &= 0x1fffff;
    return Coord{(double)(mylong * -DMS::invanglefactor + (West ? -180 : 0))};
}

uint8_t WANAddr::getColor() const {
    return back() & DMS::colormask;
}

DMS WANAddr::toDMS() const {
    return {getLat(), getLong(), getColor()};
}

DMS::DMS(Coord a, Coord b, uint8_t color) 
    : latitude{a}
    , longitude{b}
    , color{color}
{}

WANAddr DMS::asWANAddr() const {
    WANAddr addr{0, 0, 0, 0, 0, static_cast<uint8_t>(color & colormask)};
    uint32_t enc_lat = ((latitude.toDouble() < 0 ? 0.0 : 90.0) - latitude.toDouble()) * anglefactor;
    uint32_t enc_long = ((longitude.toDouble() < 0 ? 180.0 : 0.0) + longitude.toDouble()) * anglefactor;
    addr[5] |= static_cast<uint8_t>((enc_long << 5) & 0xff);  // insert low three bits
    addr[4] |= static_cast<uint8_t>((enc_long >> 3) & 0xff);  // next 8 bits
    addr[3] |= static_cast<uint8_t>((enc_long >> 11) & 0xff);  // next 8 bits
    addr[2] |= static_cast<uint8_t>((enc_long >> 19) & 0xff);  // next 8 bits
    if (longitude.toDouble() < 0) 
        addr[2] |= 0x4;
    addr[2] |= static_cast<uint8_t>((enc_lat << 3) & 0xff);  // insert low 5 bits
    addr[1] |= static_cast<uint8_t>((enc_lat >> 5) & 0xff);  // insert next 8 bits
    addr[0] |= static_cast<uint8_t>((enc_lat >> 13) & 0xff);  // insert next 8 bits
    if (latitude.toDouble() < 0) 
        addr[0] |= 0x80;

    return addr;
}

std::ostream& operator<<(std::ostream& out, const DMS& dms) {
    return out << dms.latitude << (dms.latitude.toDouble() >= 0 ? 'N' : 'S')
                << ", " << dms.longitude << (dms.longitude.toDouble() >= 0 ? 'E' : 'W')
                << " C " << static_cast<unsigned>(dms.color);
}

std::ostream& operator<<(std::ostream& out, const WANAddr& addr) {
    auto it = addr.begin();
    auto savedflags = out.setf(std::ios::hex);
    out << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned>(*it);
    for (++it ; it != addr.end(); ++it) {
        out << ':' << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned>(*it);
    }
    out.setf(savedflags);
    return out;
}
}
