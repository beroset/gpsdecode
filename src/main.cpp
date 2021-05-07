#include "WANAddr.h"
#include <iostream>

int main() {
    using namespace amrnet;
    // create a 6-byte address
    WANAddr addr{ 0x4F, 0x93, 0x1E, 0x20, 0x9d, 0xe0};
    // print it as hex
    std::cout << addr << '\n';
    // create the Degree/Minutes/Seconds version
    DMS dms{addr};
    // now print that version as degrees, minutes, seconds
    std::cout << dms << '\n';
    // use an alternate constructor
    // for these, true=North or East
    // and the last argument is a 5-bit number to distinguish among
    // up to 32 devices located at the same coordinates
    DMS antarctica{{ 88, 56, 47, 451, false}, {  2, 33, 30, 388, true }, 0};
    // print that and its WANaddr equivalent
    std::cout << antarctica.toWANAddr() << '\n' << antarctica << '\n';
    // now print the latitude and longitude
    std::cout << antarctica.getLat() << ", " << antarctica.getLong() << '\n';
    // now print the latitude and longitude as float
    std::cout << antarctica.getLat().toFloat() << ", " 
              << antarctica.getLong().toFloat() << '\n';
}
