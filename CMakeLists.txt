cmake_minimum_required(VERSION 4.0)

# Project name and version
project(DependencyExample VERSION 1.0
                          LANGUAGES CXX)

# Add the executable
add_executable(yamlexample src/main.cpp)

# Specify the C++ standard
target_compile_features(yamlexample PUBLIC cxx_std_11)

find_package(yaml-cpp REQUIRED)
target_link_libraries(yamlexample PRIVATE yaml-cpp)
