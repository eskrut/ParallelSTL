project(t-lutz)
cmake_minimum_required(VERSION 3.0)

if(DEFINED T_LUTZ_ROOT)
    set(CMAKE_CXX_FLAGS "-std=c++1z -I${T_LUTZ_ROOT}/include ${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "-pthread ${CMAKE_EXE_LINKER_FLAGS}")
else()
    message( FATAL_ERROR "T_LUTZ_ROOT should specify path to Thibaut Lutz ParallelSTL project [http://github.com/t-lutz/ParallelSTL]")
endif()

foreach(test_name sort for_each)
    add_executable(${PROJECT_NAME}_${test_name} ${test_name}.cpp
        ../testdatagenerators.hpp
        ../stopwatch.h
        )
    add_test(NAME ${PROJECT_NAME}_${test_name} COMMAND ${PROJECT_NAME}_${test_name})
endforeach()
