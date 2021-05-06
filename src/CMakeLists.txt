cmake_minimum_required(VERSION 3.15)
set(EXECUTABLE_NAME "gpsdecode")

add_library(Coord STATIC WANAddr.cpp)
target_compile_features(Coord PUBLIC cxx_std_20)

add_executable(${EXECUTABLE_NAME} main.cpp)
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC Coord)

INSTALL(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
