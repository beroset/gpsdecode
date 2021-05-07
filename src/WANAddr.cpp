#include "WANAddr.h"
#include <iomanip>
#include <cmath>

namespace amrnet {

inline namespace version_1 {
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
    return out << dms.getLat() << (dms.getLat().toFloat() >= 0 ? 'N' : 'S')
                << ", " << dms.getLong() << (dms.getLong().toFloat() >= 0 ? 'E' : 'W')
                << " C " << static_cast<unsigned>(dms.getColor());
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

} // version_1
} // amrnet
