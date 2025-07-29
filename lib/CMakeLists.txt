cmake_minimum_required(VERSION 4.0)

set(CmpWrapper_COMPRESSION_LIBRARY "Brotli" CACHE STRING
  "Indicate which compression method to use (Brotli or ZSTD)")

# Project name and version
project(CmpWrapper VERSION 1.0.0
  LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add the library
add_library(CmpWrapper)

# Create an alias for the library
add_library(CmpWrapper::CmpWrapper ALIAS CmpWrapper)

# Add the library's source files
target_sources(CmpWrapper
  PRIVATE src/cmpwrapper.cpp)

# Specify the C++ standard
target_compile_features(CmpWrapper PUBLIC cxx_std_11)

set_target_properties(CmpWrapper
  PROPERTIES VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR})

# Set the include directories
target_include_directories(
  CmpWrapper
  PRIVATE src
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)







include(FetchContent)

if ( CmpWrapper_COMPRESSION_LIBRARY STREQUAL "Brotli" )
  FetchContent_Declare(
    brotli
    GIT_REPOSITORY https://github.com/google/brotli.git
    GIT_TAG        v1.1.0
  )
  FetchContent_MakeAvailable(brotli)
  message(STATUS "brotli_SOURCE_DIR: ${brotli_SOURCE_DIR}")
  target_link_libraries(CmpWrapper PRIVATE brotlienc)
  target_compile_definitions(CmpWrapper PUBLIC __use_brotli=1)
elseif( CmpWrapper_COMPRESSION_LIBRARY STREQUAL "ZSTD" )
  find_library(ZSTD_LIB zstd REQUIRED)
  message(STATUS "ZSTD_LIB: ${ZSTD_LIB}")
  target_link_libraries(CmpWrapper PRIVATE ${ZSTD_LIB})
  target_compile_definitions(CmpWrapper PUBLIC __use_zstd=1)
else()
  message(FATAL_ERROR "Unsupported value of CmpWrapper_COMPRESSION_LIBRARY:
    ${CmpWrapper_COMPRESSION_LIBRARY}")
endif()

if ( BUILD_SHARED_LIBS )
  set(CmpWrapper_STATIC_BUILD OFF)
else()
  set(CmpWrapper_STATIC_BUILD ON)
endif()


######################################################
# BEGIN INSTALL SECTION
######################################################
include(GNUInstallDirs)

# Set install locations
install(TARGETS CmpWrapper
        EXPORT CmpWrapperTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install Brotli targets if Brotli is used
if (CmpWrapper_COMPRESSION_LIBRARY STREQUAL "Brotli")
  install(TARGETS brotlienc brotlidec brotlicommon
          EXPORT CmpWrapperTargets
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY ${brotli_SOURCE_DIR}/c/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Create the CMake configuration files
include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CmpWrapperConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CmpWrapperConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/CmpWrapper)

write_basic_package_version_file(
  "CmpWrapperConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/CmpWrapperConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/CmpWrapperConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/CmpWrapper)

install(
  EXPORT CmpWrapperTargets
  FILE CmpWrapperTargets.cmake
  NAMESPACE CmpWrapper::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/CmpWrapper)
