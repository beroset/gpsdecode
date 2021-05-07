#include "WANAddr.h"
#include <iomanip>
#include <cmath>

namespace amrnet {


WANAddr DMS::asWANAddr() const {
    WANAddr addr{0, 0, 0, 0, 0, static_cast<uint8_t>(color & WANAddr::colormask)};
    uint32_t enc_lat = ((latitude.toFloat() < 0 ? 0.0 : 90.0) - latitude.toFloat()) * anglefactor;
    uint32_t enc_long = ((longitude.toFloat() < 0 ? 180.0 : 0.0) + longitude.toFloat()) * anglefactor;
    addr[5] |= static_cast<uint8_t>((enc_long << 5) & 0xff);  // insert low three bits
    addr[4] |= static_cast<uint8_t>((enc_long >> 3) & 0xff);  // next 8 bits
    addr[3] |= static_cast<uint8_t>((enc_long >> 11) & 0xff);  // next 8 bits
    addr[2] |= static_cast<uint8_t>((enc_long >> 19) & 0xff);  // next 8 bits
    if (longitude.toFloat() < 0) 
        addr[2] |= 0x4;
    addr[2] |= static_cast<uint8_t>((enc_lat << 3) & 0xff);  // insert low 5 bits
    addr[1] |= static_cast<uint8_t>((enc_lat >> 5) & 0xff);  // insert next 8 bits
    addr[0] |= static_cast<uint8_t>((enc_lat >> 13) & 0xff);  // insert next 8 bits
    if (latitude.toFloat() < 0) 
        addr[0] |= 0x80;

    return addr;
}

std::ostream& operator<<(std::ostream& out, const Coord& coord) {
    myfloat a = coord.toFloat();
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

std::ostream& operator<<(std::ostream& out, const DMS& dms) {
    return out << dms.latitude << (dms.latitude.toFloat() >= 0 ? 'N' : 'S')
                << ", " << dms.longitude << (dms.longitude.toFloat() >= 0 ? 'E' : 'W')
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
