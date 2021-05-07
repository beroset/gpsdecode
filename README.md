# Introduction #
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
### Example output
When linked with this library, that prints the following:
```
4f:93:1e:20:9d:e0
34°2'56.831"N, 84°15'59.745"W C 0
fe:80:70:0e:8e:00
88°56'47.450"S, 2°33'30.388"E C 0
88°56'47.450", 2°33'30.388"
-88.9465, 2.55844
```

# How to build this software #

For those used to [CMake](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) projects, this will be a very familiar process.  Once you have the source code in a main directory, `gpsdecode` create a `build` directory under that and navigate to it.

Then invoke cmake to specify the source and build directories as well as the build tool.  If you have the cross-platform [Ninja](https://ninja-build.org/) build system installed, run the command `cmake -S .. -B . -G "Ninja"` to create the build framework via cmake.  

If that occurs without error, then the next step is to build:  `cmake --build .` will do this.  By default, only the library and the example program are built.  

If you wish to build the documentation, you will need a few additional pieces of software, including [Doxygen](https://www.doxygen.nl/index.html) and some other tools if you want the PDF version, for example.

The software also has optional set of unit tests which use the [googletest](https://github.com/google/googletest) testing framework.

These can be independently enabled or disabled.  To configure the build system to build both, you can use the command `cmake -S .. -B . -G "Ninja" -DBUILD_DOCS=1 -DBUILD_TESTING=1`.  After that you can build the artifacts.

```
cmake --build . 
cmake --build . --target test
cmake --build . --target doc
cmake --build . --target pdf
```

You must build the software before building the tests because the tests rely on the created library.  You must also create `doc` (the HTML documents) before you create `pdf` (the PDF version).  

Assuming you have followed these instruction exactly, and have run all of these build targets in that order, the library and the example application are in `build/src`, the tests are in `build/test` and the documentation in HTML and PDF format is in `build/doc/html` (start with `index.html` located there) and `build/doc/latex` (as `refman.pdf`).

# How to use this software #

If you have followed the instructions for [How to build this software](#how-to-build-this-software), then you will have the sample executable file in `build/src/gpsdecode`.  By itself, it is not particularly useful except as an illustration.  

If you wish to use the library in your own software, you can copy the library (named `build/src/libCoord.a` under Linux) and the header file `src/WANAddr.h` to your project and use them directly.  Note that these locations are all *relative* paths with respect to the root directory for the project, typically named `gpsdecode`.  

For example if you wanted to start a new project based on the example, you could create a separate project folder somewhere, say `myproject`, and copy `build/src/libCoord.a`, `src/WANAddr.h` and `src/main.cpp` to that directory.  So at this point your directory has three files in it and looks like this:


```

├── libCoord.a
├── main.cpp
└── WANAddr.h
```

Under Linux, you can build and link these with gcc using the following command:

`g++ -std=c++17 main.cpp -L. -lCoord -o example`

Because the header file uses C++17 constructs, you must specify `-std=c++17` (or later, such as `-std=c++20`) on the command line to let the compiler know.  The file you're compiling is `main.cpp` which comes next.  We tell `g++` to look in the current directory for libraries to link (`-L.`) and specify that we wish to link with the `Coord` library `-lCoord` (note that the compiler assumes every library name begins with `lib` so it will look for either `libCoord.a` or `libCoord.so` which would be a dynamically linked version).  Finally we let it know that the output file should be named `example` (`-o example`).  

This will create a new file named `example` which you can run as `./example`.  It does nothing more than print a few coordinates and the output should match the [documented output](@ref #example-output). 
