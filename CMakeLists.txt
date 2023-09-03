cmake_minimum_required(VERSION 3.19.0)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

option(ZB_BUILD_TESTS "Build tests" ON)
option(ZB_BUILD_EXAMPLES "Build examples" ON)

set(ZB_VERSION_MAJOR 0)
set(ZB_VERSION_MINOR 6)
set(ZB_VERSION_PATCH 0)
set(ZB_VERSION "${ZB_VERSION_MAJOR}.${ZB_VERSION_MINOR}.${ZB_VERSION_PATCH}")

project(zerobase_cpp VERSION ${ZB_VERSION} LANGUAGES C CXX)

# Third party dependencies
add_subdirectory(vendor)

# Main library
add_library(zerobase_cpp)
file(GLOB_RECURSE SOURCES "src/**.cpp")

message(STATUS "Configuring zerobase_cpp ${ZB_VERSION}")

target_sources(zerobase_cpp PRIVATE ${SOURCES})
target_include_directories(zerobase_cpp PUBLIC include PRIVATE src/include)
target_link_libraries(zerobase_cpp PUBLIC cppzmq)

if(ZB_BUILD_TESTS)
    message(STATUS "Configuring tests")

    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests")
endif()

if(ZB_BUILD_EXAMPLES)
    message(STATUS "Configuring examples")

    add_subdirectory(examples)
else()
    message(STATUS "Skipping examples")
endif()