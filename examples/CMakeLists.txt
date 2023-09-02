cmake_minimum_required(VERSION 3.19.0)

SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)

project(zerobase_cpp VERSION ${ZB_VERSION} LANGUAGES C CXX)

foreach(current_example "basic" "multi_transport_basic" "multi_transport_advanced")
    message(STATUS "  Configuring ${current_example}")
    file(GLOB BASIC_SOURCES "${current_example}.cpp")
    add_executable(${current_example})

    target_sources(${current_example} PRIVATE ${BASIC_SOURCES})
    target_link_libraries(${current_example} PRIVATE zerobase_cpp)
    
endforeach()