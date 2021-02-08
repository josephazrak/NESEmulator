cmake_minimum_required(VERSION 3.17)
project(NESEmulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(NESEmulator main.cpp system/mos6502.cpp system/mos6502.hpp system/ram.cpp system/ram.hpp)

INCLUDE_DIRECTORIES(/usr/local/opt/allegro/include)
LINK_DIRECTORIES(/usr/local/opt/allegro/lib)

file(GLOB LIBRARIES "/usr/local/opt/allegro/lib/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

TARGET_LINK_LIBRARIES(NESEmulator ${LIBRARIES})