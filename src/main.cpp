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
