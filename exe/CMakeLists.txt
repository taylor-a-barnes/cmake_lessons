cmake_minimum_required(VERSION 4.0)

# Project name and version
project(CmpExecutable VERSION 1.0)

# Add the executable
add_executable(cmpexecutable src/main.cpp)

# Specify the C++ standard
target_compile_features(cmpexecutable PUBLIC cxx_std_11)

# Link against the CmpWrapper library
find_package(CmpWrapper 1.0 REQUIRED)
target_link_libraries(cmpexecutable PUBLIC CmpWrapper::CmpWrapper)

# Link against yaml-cpp
include(FetchContent)
find_package(yaml-cpp)
if ( yaml-cpp_FOUND )
  target_link_libraries(cmpexecutable PRIVATE yaml-cpp)
else()
  FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        2f86d13775d119edbb69af52e5f566fd65c6953b
  )
  FetchContent_MakeAvailable(yaml-cpp)
  target_link_libraries(cmpexample PRIVATE yaml-cpp)
endif()

