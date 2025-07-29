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
