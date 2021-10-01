cmake_minimum_required(VERSION 3.0.0)

project(generate_analysis)
set(CMAKE_CXX_STANDARD 17)


set(SOURCES ./sources/Analysis.cpp ./sources/HPGe.cpp ./sources/FC.cpp)
set(HEADERS ./headers/Analysis.h ./headers/HPGe.h ./headers/FC.h )
include_directories(./headers)


add_executable(generate_analysis generate_analysis.cpp ${SOURCES} ${HEADERS})

