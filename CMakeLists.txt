cmake_minimum_required(VERSION 4.0)

set(CmpExample_COMPRESSION_LIBRARY "Brotli" CACHE STRING
  "Indicate which compression method to use (Brotli or ZSTD)")

# Project name and version
project(CmpExample
  LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add the executable
add_executable(cmpexample src/main.cpp)

include(FetchContent)

find_package(yaml-cpp)
if ( yaml-cpp_FOUND )
  target_link_libraries(cmpexample PRIVATE yaml-cpp)
else()
  FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        2f86d13775d119edbb69af52e5f566fd65c6953b
  )
  FetchContent_MakeAvailable(yaml-cpp)
  target_link_libraries(cmpexample PRIVATE yaml-cpp)
endif()


if ( CmpExample_COMPRESSION_LIBRARY STREQUAL "Brotli" )
  FetchContent_Declare(
    brotli
    GIT_REPOSITORY https://github.com/google/brotli.git
    GIT_TAG        v1.1.0
  )
  FetchContent_MakeAvailable(brotli)
  message(STATUS "brotli_SOURCE_DIR: ${brotli_SOURCE_DIR}")
  target_link_libraries(cmpexample PRIVATE brotlienc)
  target_compile_definitions(cmpexample PUBLIC __use_brotli=1)
elseif( CmpExample_COMPRESSION_LIBRARY STREQUAL "ZSTD" )
  find_library(ZSTD_LIB zstd REQUIRED)
  message(STATUS "ZSTD_LIB: ${ZSTD_LIB}")
  target_link_libraries(cmpexample PRIVATE ${ZSTD_LIB})
  target_compile_definitions(cmpexample PUBLIC __use_zstd=1)
else()
  message(FATAL_ERROR "Unsupported value of CmpExample_COMPRESSION_LIBRARY:
    ${CmpExample_COMPRESSION_LIBRARY}")
endif()
