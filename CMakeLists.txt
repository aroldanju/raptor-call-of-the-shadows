cmake_minimum_required(VERSION 3.14)

project(raptor C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)
add_subdirectory(vendor/ini)
add_subdirectory(vendor/res)
add_subdirectory(vendor/engine)

add_subdirectory(src/cli)
