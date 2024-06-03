cmake_minimum_required(VERSION 3.0.0)
project(HamArc VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED 17)

add_executable(HamArc lib/ArgParser.cpp lib/Archiver.cpp lib/FileNameOperator.cpp lib/Hamming.cpp)
