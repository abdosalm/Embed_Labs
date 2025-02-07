cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(L3)

target_sources(app PRIVATE src/main.c)


# add all the source files (.c) files to be included in our build
# the '.c' files exists in 2 place (BMI323_SensorAPI/) and (BMI323_SensorAPI/examples/common)
# documentation can be found at: https://cmake.org/cmake/help/latest/command/target_sources.html 
target_sources(app PRIVATE BMI323_SensorAPI/bmi3.c; BMI323_SensorAPI/bmi323.c;
                    BMI323_SensorAPI/examples/common/common.c)

# include all the header files
# documentation can be found at: https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories 
target_include_directories(app PRIVATE BMI323_SensorAPI; BMI323_SensorAPI/examples/common)