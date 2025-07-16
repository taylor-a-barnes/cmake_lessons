cmake_minimum_required(VERSION 4.0)

# Project name and version
project(BrotliExample VERSION 1.0
                          LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add the executable
add_executable(brotliexample src/main.cpp)

include(FetchContent)

find_package(yaml-cpp)
if ( yaml-cpp_FOUND )
  target_link_libraries(brotliexample PRIVATE yaml-cpp)
else()
  FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        2f86d13775d119edbb69af52e5f566fd65c6953b
  )
  FetchContent_MakeAvailable(yaml-cpp)
  target_link_libraries(brotliexample PRIVATE yaml-cpp)
endif()

FetchContent_Declare(
  brotli
  GIT_REPOSITORY https://github.com/google/brotli.git
  GIT_TAG        v1.1.0
)
FetchContent_MakeAvailable(brotli)

message(STATUS "brotli_SOURCE_DIR: ${brotli_SOURCE_DIR}")
target_link_libraries(brotliexample PRIVATE brotlienc)
