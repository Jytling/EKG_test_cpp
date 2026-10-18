cmake_minimum_required(VERSION 3.29)
project(somethingtodo)

set(CMAKE_CXX_STANDARD 14)

add_executable(somethingtodo cmake-build-debug/samples.csv CZ_1_G_G_C.cpp)
