cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(USE_FOLDERS ON)
project(Pantheon LANGUAGES CXX C)

# Setup cpp version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Setup output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/Engine)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/Engine)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/Engine)

# Add module folder
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

enable_testing()

set(PANTHEON_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
include(CopyResources)

set(CMAKE_FOLDER deps)
add_subdirectory(Dependencies)

set(CMAKE_FOLDER Pantheon)
add_subdirectory(Source)
add_subdirectory(Resources)

if (MSVC AND DEFINED STARTUP_PROJECT)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${STARTUP_PROJECT})
endif()