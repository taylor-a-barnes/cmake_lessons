cmake_minimum_required(VERSION 4.0)

# Project name and version
project(BrotliExample VERSION 1.0
  LANGUAGES CXX)

# Add the executable
add_executable(brotliexample src/main.cpp)

target_compile_features(brotliexample PUBLIC cxx_std_11)

find_package(yaml-cpp REQUIRED)
target_link_libraries(brotliexample PRIVATE yaml-cpp)
