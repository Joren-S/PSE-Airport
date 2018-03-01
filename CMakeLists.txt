cmake_minimum_required(VERSION 3.6)
project(ProjectVliegveld)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES system.cpp headers/airports.h headers/runways.h headers/types.h headers/DesignByContract.h headers/airplanes.h src/airplanes.cpp src/airports.cpp src/runways.cpp tinyXML/tinystr.cpp tinyXML/tinyxml.cpp tinyXML/tinyxmlerror.cpp tinyXML/tinyxmlparser.cpp)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES headers/airports.h headers/runways.h headers/types.h headers/DesignByContract.h headers/airplanes.h src/airplanes.cpp src/airports.cpp src/runways.cpp system_test.cpp)

# Create RELEASE target
add_executable(pv ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(pv_debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(pv_debug gtest)
