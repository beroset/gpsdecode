#ifndef WANADDR_H
#define WANADDR_H
#include <iostream>
#include <array>
#include <cmath>

namespace amrnet {

inline namespace version_1 {

using myfloat = double;

class Coord {
public:
    constexpr Coord(uint8_t degrees, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, bool NE)
        : a{(NE ? 1.0 : -1.0) * (
        (milliseconds / 3600'000.0) +
        (seconds / 3600.0) + 
        (minutes / 60.0) + 
        degrees
        )}
    {}
    constexpr Coord(myfloat a = 0) : a{a} {};

    friend std::ostream& operator<<(std::ostream& out, const Coord& coord);
    constexpr myfloat toFloat() const { return a; } 
    constexpr myfloat operator()(myfloat) const { return a; }
    // these only have a maximum of 21 significant bits, 
    // so any difference less than that is essentially zero
    static constexpr myfloat epsilon{1.0 / 0x20'0000};
private:
    myfloat a;
};

/*
 * Format of WANAddr, which is a 48-bit quantity
 *
 *          |         |         |         |         |
 * 4444 4444 3333 3333 3322 2222 2222 1111 1111 1100 0000 0000
 * 7654 3210 9876 5432 1098 7654 3210 9876 5432 1098 7654 3210
 *
 * Sttt tttt tttt tttt tttt tWgg gggg gggg gggg gggg gggc cccc
 * ||                        ||                         |
 * ||                        ||                         +- color
 * ||                        ||
 * ||                        |+-- longitude (unsigned), 21 bits
 * ||                        |
 * ||                        +--- 1 = W, 0 = E longitude
 * ||
 * |+-- latitude (unsigned), 20 bits
 * |
 * +--- 1 = S, 0 = N latitude
 * 
 * Latitude and longitude are scaled such that a full scale 
 * 180 degree value is 0x20'0000 (i.e. 21 bits)
 *
 * range of latitude is [-90, +90]
 * range of longitude is [-180, +180]
 * range of color = [0, 32)
 *
 * Color is used to distinguish multiple nodes at the same location.
 * 
 */
struct WANAddr : public std::array<uint8_t, 6> {
    friend std::ostream& operator<<(std::ostream& out, const WANAddr& addr);
    constexpr Coord getLat() const {
        constexpr std::size_t startbyte{0};
        constexpr std::size_t discardbits{11};
        constexpr uint32_t mask{1U << 20};
        const auto [mylat, South] = extract(startbyte, discardbits, mask);
        return Coord{(myfloat)(mylat * invanglefactor + (South ? 0 : 90))};
    }

    constexpr Coord getLong() const {
        constexpr std::size_t startbyte{2};
        constexpr std::size_t discardbits{5};
        constexpr uint32_t mask{1U << 21};
        const auto [mylong, West] = extract(startbyte, discardbits, mask);
        return Coord{(myfloat)(mylong * -invanglefactor + (West ? -180 : 0))};
    }

    constexpr uint8_t getColor() const {
        return back() & colormask;
    }

    constexpr std::pair<uint32_t, bool> extract(std::size_t startbyte, std::size_t discardbits, uint32_t mask) const {
        uint32_t mylat{0};
        auto count{sizeof(mylat)};
        for (auto it{begin() + startbyte}; count; --count) {
            mylat = (mylat << 8) | *it++;
        } 
        // we only need 20 bits of the 32
        mylat >>= discardbits;
        // remember the sign bit
        bool South{(mylat & mask) != 0};
        // and zero the sign bit
        mylat &= (mask - 1);
        return {mylat, South};
    }
    static constexpr uint8_t colormask = 0x1f;
    static constexpr myfloat invanglefactor{-180.0 / 0x20'0000};
};

class DMS {
public:
    constexpr DMS(Coord a, Coord b, uint8_t color) 
        : latitude{a}
        , longitude{b}
        , color{color}
    {}
    constexpr DMS(const WANAddr& addr) 
        : latitude{addr.getLat()}
        , longitude{addr.getLong()}
        , color{addr.getColor()}
    {}
    constexpr WANAddr toWANAddr() const {
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
    friend std::ostream& operator<<(std::ostream& out, const DMS& dms);
    friend constexpr bool operator==(const DMS& lhs, const DMS& rhs);
    constexpr Coord getLat() const { return latitude; }
    constexpr Coord getLong() const { return longitude; }
    constexpr uint8_t getColor() const { return color; }
private:
    static constexpr myfloat anglefactor{0x20'0001 / 180.0};

    Coord latitude;
    Coord longitude;
    uint8_t color;
};

inline constexpr bool operator==(const Coord& lhs, const Coord& rhs) { 
    return std::abs(lhs.toFloat() - rhs.toFloat()) < Coord::epsilon;
}

inline constexpr bool operator==(const DMS& lhs, const DMS& rhs) { 
    return lhs.latitude == rhs.latitude 
        && lhs.longitude == rhs.longitude
        && lhs.color == rhs.color;
}

// in C++20, these will no longer be necessary to write
inline constexpr bool operator!=(const DMS& lhs, const DMS& rhs) { 
    return !(lhs == rhs);
}

inline constexpr bool operator!=(const Coord& lhs, const Coord& rhs) { 
    return !(lhs == rhs);
}

} // version_1
} // amrnet

#endif // WANADDR_H
