cmake_minimum_required(VERSION 3.16)
include_directories( ${CMAKE_HOME_DIRECTORY}/src )
find_package(Threads)
find_package(GTest)
add_executable(CoordTest CoordTest.cpp)

target_link_libraries(CoordTest Coord ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(LocationTests CoordTest)
