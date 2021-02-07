cmake_minimum_required(VERSION 3.17)
project(NESEmulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(NESEmulator main.cpp system/mos6502.cpp system/mos6502.hpp system/ram.cpp system/ram.hpp)