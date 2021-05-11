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
### Data structure

The WAN Address is a 48-bit quantity, encoded as follows:

                   4                   3                   2                   1  
     7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |S|                Latitude               |W|                  Longitude              |  Color  | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 - The `S` bit is 1 if the latitude is South, 0 for North
 - The `W` bit is 1 if the longitude is West, 0 for East

Latitude and longitude are scaled such that a full scale, a 180 degree value is 0x20'0000 (i.e. 21 bits).  The range of latitude is [-90, +90], the range of longitude is [-180, +180].  Because the range of latitude is half that of longitude (and half of full scale) we can use only 20 bits to encode it with no loss of precision.  Color is used to distinguish multiple nodes at the same location and its range is [0, 31].

## Further reading

[How to build this software](@ref building)

[How to use this software](@ref using)
