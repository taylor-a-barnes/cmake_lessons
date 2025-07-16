cmake_minimum_required(VERSION 4.0)

# Project name and version
project(BrotliExample VERSION 1.0
                          LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add the executable
add_executable(brotliexample src/main.cpp)

find_package(yaml-cpp REQUIRED)
target_link_libraries(brotliexample PRIVATE yaml-cpp)
