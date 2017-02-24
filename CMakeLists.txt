project(parallelSTL)
cmake_minimum_required(VERSION 3.0)

set(MEMORY_TO_ALLOC "1.0" CACHE STRING "Ammount of memory in Gb to allocate for test data")
add_definitions(-DMEMORY_TO_ALLOC=${MEMORY_TO_ALLOC})

enable_testing()

option(MAKE_T_LUTZ "Make tests with Thibaut Lutz implementation" ON)
if(MAKE_T_LUTZ)
    message("See http://github.com/t-lutz/ParallelSTL")
    add_subdirectory(t-lutz)
endif()

option(MAKE_HPX "Make tests with HPX implementation" ON)
if(MAKE_HPX)
    message("See http://stellar-group.github.io/hpx/docs/html/hpx/manual/parallel.html")
    add_subdirectory(hpx)
endif()

option(MAKE_GCC "Make tests with libstdc++ parallel mode" ON)
if(MAKE_GCC)
    message("See http://gcc.gnu.org/onlinedocs/libstdc++/manual/parallel_mode.html"
        "\nNote. This is not Parallelism implementation"
        "but standalone libstdc++ features similar to proposed ones"
        )
    add_subdirectory(gcc)
endif()
