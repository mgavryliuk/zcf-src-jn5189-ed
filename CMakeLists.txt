cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_VERBOSE_MAKEFILE OFF)

project(AQARA_H1)
enable_language(ASM)
enable_language(C)

add_subdirectory(src)
