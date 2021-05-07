#include "WANAddr.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>

int main() {
    std::vector<std::pair<amrnet::DMS, amrnet::WANAddr>> samples{ 
        { {{ 88, 56, 47, 451, false}, {  2, 33, 30, 388, true }, 0},    { 0xFE, 0x80, 0x70, 0x0e, 0x8E, 0x00} },
        { {{ 88, 56, 47, 451, false}, { 10, 42, 37,  68, true }, 0},    { 0xFE, 0x80, 0x70, 0x3c, 0xee, 0x00} },
        { {{ 88, 56, 47, 451, false}, {135, 13, 13, 488, true }, 0},    { 0xFE, 0x80, 0x73, 0x01, 0x41, 0x00} },
        { {{ 34,  2, 56, 832, true }, { 84, 15, 59, 745, false}, 0},    { 0x4F, 0x93, 0x1E, 0x20, 0x9d, 0xe0} },
        { {{ 35,  4, 59, 858, true }, {160, 10,  0,  25, false}, 0},    { 0x4e, 0x1a, 0x94, 0x70, 0xd4, 0x60} },
        { {{ 35, 14, 59, 918, true }, {159, 59, 59, 966, false}, 0},    { 0x4d, 0xdd, 0xe4, 0x71, 0xc7, 0x20} },
        { {{ 35, 19, 59, 947, true }, {160, 20,  0,  85, false}, 0},    { 0x4d, 0xbf, 0x8c, 0x6f, 0xe1, 0xa0} },
        { {{ 46, 36, 15, 650, true }, { 94, 16,  7, 641, false}, 2},    { 0x3d, 0xb7, 0xe5, 0xe7, 0xb7, 0x22} },
        { { 33.179569 , -96.698913, 0},                                 { 0x50, 0xcf, 0xb5, 0xD9, 0xE4, 0x00} }, 
        { { 33.1763075, -96.699600, 0},                                 { 0x50, 0xD0, 0xE5, 0xD9, 0xE3, 0x00} }, 
    };
    
    for (const auto [coord, addr] : samples) {
        std::cout << addr  << '\t' << coord << "\t( " 
            << std::setprecision(9) << coord.getLat().toFloat() << ", " << coord.getLong().toFloat() << ")\n";
        const auto calc = coord.asWANAddr();
        if (calc != addr) 
            std::cout << "\tCalculated " << calc << '\n';
        const amrnet::DMS calcDMS{addr};
        if (calcDMS != coord) 
            std::cout << "\tCalculated\t" << calcDMS << '\n';
    }
}
