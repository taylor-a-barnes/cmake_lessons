cmake_minimum_required(VERSION 3.22)

# Project name and version
project(InitialExample VERSION 1.0
		       LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add the executable
add_executable(yamlexample src/main.cpp)

find_package(yaml-cpp REQUIRED)
target_link_libraries(yamlexample PRIVATE yaml-cpp)
