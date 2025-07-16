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

include(FetchContent)
FetchContent_Declare(
  brotli
  GIT_REPOSITORY https://github.com/google/brotli.git
  GIT_TAG        v1.1.0
)
FetchContent_MakeAvailable(brotli)

message(STATUS "brotli_SOURCE_DIR: ${brotli_SOURCE_DIR}")

target_link_libraries(brotliexample PRIVATE brotlienc)
target_include_directories(brotliexample PRIVATE ${brotli_SOURCE_DIR}/c/include})
