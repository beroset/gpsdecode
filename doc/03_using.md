# How to use this software {#using} #

If you have followed the instructions for [How to build this software](@ref building), then you will have the sample executable file in `build/src/gpsdecode`.  By itself, it is not particularly useful except as an illustration.  

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

This will create a new file named `example` which you can run as `./example`.  It does nothing more than print a few coordinates and the output should match the [documented output](@ref mainpage). 

## Further reading

[Introduction](@ref mainpage)

[How to build this software](@ref building)
