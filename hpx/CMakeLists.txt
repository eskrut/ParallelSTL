project(hpx)
cmake_minimum_required(VERSION 3.0)

find_package(HPX REQUIRED)

include_directories(${HPX_INCLUDE_DIRS})
link_directories(${HPX_LIBRARY_DIR})

foreach(test_name sort for_each exception_handling)
    add_executable(${PROJECT_NAME}_${test_name} ${test_name}.cpp
        ../testdatagenerators.hpp
        ../stopwatch.h
        )
    target_link_libraries(${PROJECT_NAME}_${test_name} ${HPX_LIBRARIES})
    add_test(NAME ${PROJECT_NAME}_${test_name} COMMAND ${PROJECT_NAME}_${test_name})
endforeach()
