# Introduction {#mainpage} #
Beginning in the late 1980s and continuing to today, some Automatic Meter Reading (AMR) systems employed a geographic routing scheme to route RF communications packets through a mesh network.  Some systems used an encoding of the latitude and longitude as their Wide Area Network (WAN) address.  This C++17 library performs the encoding and decoding functions for this type of address.

# Purpose
This is a C++17 library to encode and decode such WAN addresses to and from the traditional latitude and longitude system.

## Example

```
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
    DMS antarctica{{ 88, 56, 47, 451, false}, {  2, 33, 30, 388, true }, 0};
    // print that and its WANaddr equivalent
    std::cout << antarctica.toWANAddr() << '\n' << antarctica << '\n';
    // now print the latitude and longitude
    std::cout << antarctica.getLat() << ", " << antarctica.getLong() << '\n';
    // now print the latitude and longitude as float
    std::cout << antarctica.getLat().toFloat() << ", " 
              << antarctica.getLong().toFloat() << '\n';
}

``` 

When linked with this library, that prints the following:
```
4f:93:1e:20:9d:e0
34°2'56.831"N, 84°15'59.745"W C 0
fe:80:70:0e:8e:00
88°56'47.450"S, 2°33'30.388"E C 0
88°56'47.450", 2°33'30.388"
-88.9465, 2.55844
```

## Further reading

[How to build this software](@ref building)

[How to use this software](@ref using)
