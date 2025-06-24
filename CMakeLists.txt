cmake_minimum_required(VERSION 4.0)

# Project name and version
project(InitialExample VERSION 1.0
		       LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add the executable
add_executable(helloworld src/main.cpp)
