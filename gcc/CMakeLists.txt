project(gcc)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-std=c++1y ${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")

foreach(test_name sort for_each)
    add_executable(${PROJECT_NAME}_${test_name} ${test_name}.cpp
        ../testdatagenerators.hpp
        ../stopwatch.h
        )
    add_test(NAME ${PROJECT_NAME}_${test_name} COMMAND ${PROJECT_NAME}_${test_name})
endforeach()
