cmake_minimum_required(VERSION 3.19.0)

SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)

project(zerobase_cpp VERSION ${ZB_VERSION} LANGUAGES C CXX)

foreach(current_test "test_main")
    message(STATUS "  Configuring ${current_test}")
    file(GLOB BASIC_SOURCES "${current_test}.cpp")
    add_executable(${current_test})

    target_sources(${current_test} PRIVATE ${BASIC_SOURCES})
    target_link_libraries(${current_test} PRIVATE zerobase_cpp)
    
endforeach()