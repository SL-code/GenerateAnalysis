cmake_minimum_required(VERSION 3.0.0)

project(generate_analysis)
set(CMAKE_CXX_STANDARD 17)


set(SOURCES ./sources/Analysis.cpp
            ./sources/HPGe.cpp
            ./sources/FC.cpp
            ./sources/HPGe_ListSort.cpp
            ./sources/HPGe_AddMatrix.cpp)

SET(GCC_COVERAGE_COMPILE_FLAGS "-g" )

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")


add_executable(generate_analysis main.cpp ${SOURCES})
target_include_directories(generate_analysis PUBLIC ./headers)

