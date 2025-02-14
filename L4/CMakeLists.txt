cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(L4)

target_sources(app PRIVATE src/main.c)

# add all the source files (.c) files to be included in our build
# the '.c' files exists only in the src directory.
# documentation can be found at: https://cmake.org/cmake/help/latest/command/target_sources.html 
target_sources(app PRIVATE src/gpio.c)