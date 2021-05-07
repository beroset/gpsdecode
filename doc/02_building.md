# How to build this software {#building} #

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

## Further reading

[Introduction](@ref mainpage)

[How to use this software](@ref using)
