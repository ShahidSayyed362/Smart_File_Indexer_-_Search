cmake_minimum_required(VERSION 3.20)
project(SmartFileIndexer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add Include Directories
include_directories(include)

# Gather source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Exclude main.cpp from library sources
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Create a core library for testing and the main app
add_library(SmartFileIndexer_lib ${SOURCES})
target_include_directories(SmartFileIndexer_lib PUBLIC include)

# Main executable
add_executable(SmartFileIndexer src/main.cpp)
target_link_libraries(SmartFileIndexer PRIVATE SmartFileIndexer_lib)

# Testing
enable_testing()
add_subdirectory(tests)
