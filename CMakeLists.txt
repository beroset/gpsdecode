cmake_minimum_required(VERSION 3.16)
project(gpsdecode VERSION 1.0.0 LANGUAGES CXX)

# options off-by-default that you can enable
option(BUILD_DOCS "Build the documentation" OFF)
option(BUILD_TESTING "Build test suite" OFF)


if (BUILD_DOCS)
    add_subdirectory(doc)
endif (BUILD_DOCS)

if (BUILD_TESTING)
    include(CTest)
    add_subdirectory(test)
endif() 

add_subdirectory(src)

SET(CPACK_SOURCE_IGNORE_FILES "/build/;.swp;.git")
INCLUDE(InstallRequiredSystemLibraries)
include (CPack)
