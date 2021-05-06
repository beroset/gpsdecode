#ifndef WANADDR_H
#define WANADDR_H
#include <iostream>
#include <array>

namespace amrnet {
class DMS;
class Coord;

/*
 * Format of WANAddr, which is a 48-bit quantity
 *
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
 * Color is used to distinguish multiple nodes at the same location.
 * 
 */
struct WANAddr : public std::array<uint8_t, 6> {
    friend std::ostream& operator<<(std::ostream& out, const WANAddr& addr);
    Coord getLat() const;
    Coord getLong() const;
    uint8_t getColor() const;
    DMS toDMS() const;
};

class Coord {
public:
    Coord(uint8_t degrees, uint8_t minutes, uint8_t seconds, uint16_t milliseconds, bool NE);
    Coord(double a);

    friend std::ostream& operator<<(std::ostream& out, const Coord& coord);
    double toDouble() const { return a; } 
    double operator()(double) const { return a; }
    // these only have 21 significant bits, 
    // so any difference less than that is essentially zero
    static constexpr double epsilon{1.0 / 0x20'0000};
private:
    double a;
};

class DMS {
public:
    DMS(Coord a, Coord b, uint8_t color);
    WANAddr asWANAddr() const;
    friend std::ostream& operator<<(std::ostream& out, const DMS& dms);
    friend bool operator==(const DMS& lhs, const DMS& rhs);
    Coord getLat() const { return latitude; }
    Coord getLong() const { return longitude; }
    static constexpr double anglefactor{0x20'0001 / 180.0};
    static constexpr double invanglefactor{-180.0 / 0x20'0000};
    static constexpr uint8_t colormask{0x1f};
private:
    Coord latitude;
    Coord longitude;
    uint8_t color;
};

inline bool operator==(const Coord& lhs, const Coord& rhs) { 
    return (lhs.toDouble() - rhs.toDouble()) < Coord::epsilon;
}

inline bool operator==(const DMS& lhs, const DMS& rhs){ 
    return lhs.latitude == rhs.latitude 
        && lhs.longitude == rhs.longitude
        && lhs.color == rhs.color;
}

inline bool operator!=(const DMS& lhs, const DMS& rhs){
    return !operator==(lhs,rhs);
}
}

#endif // WANADDR_H
