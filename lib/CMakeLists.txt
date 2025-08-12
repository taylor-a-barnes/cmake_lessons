cmake_minimum_required(VERSION 4.0)

# Project name and version
project(CmpWrapper VERSION 1.0.0
  LANGUAGES CXX)

set(CmpWrapper_COMPRESSION_LIBRARY "Brotli" CACHE STRING
  "Indicate which compression library to use (Brotli or ZSTD)")

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
  target_compile_definitions(CmpWrapper PRIVATE __use_brotli=1)
elseif( CmpWrapper_COMPRESSION_LIBRARY STREQUAL "ZSTD" )
  find_library(ZSTD_LIB zstd REQUIRED)
  message(STATUS "ZSTD_LIB: ${ZSTD_LIB}")
  target_link_libraries(CmpWrapper PRIVATE ${ZSTD_LIB})
  target_compile_definitions(CmpWrapper PRIVATE __use_zstd=1)
else()
  message(FATAL_ERROR "Unsupported value of CmpWrapper_COMPRESSION_LIBRARY:
    ${CmpWrapper_COMPRESSION_LIBRARY}")
endif()




#############################################
# BEGIN INSTALL SECTION
#############################################
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
endif()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Create the CMake configuration files
include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CmpWrapperConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CmpWrapperConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CmpWrapper)

write_basic_package_version_file(
  "CmpWrapperConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/CmpWrapperConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/CmpWrapperConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CmpWrapper)

install(
  EXPORT CmpWrapperTargets
  FILE CmpWrapperTargets.cmake
  NAMESPACE CmpWrapper::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CmpWrapper)




set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Organization Name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CPack example project")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE)

set(CPACK_PACKAGE_CONTACT "example@test.org")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "My Name")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
include(CPack)
