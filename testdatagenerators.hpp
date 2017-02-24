#ifndef TESTDATAGENERATORS_HPP
#define TESTDATAGENERATORS_HPP

#include <stdlib.h>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>

const float memoryToAlloc = MEMORY_TO_ALLOC;

std::vector<size_t> makeShuffledVector (float sizeInGb = 0) {
    if(sizeInGb == 0) sizeInGb = memoryToAlloc;
    const size_t length = sizeInGb /*Gb*/
            * 1024ull /*Mb*/
            * 1024 /*Kb*/
            * 1024 /*b*/
            / sizeof(size_t) /*count*/;
    std::vector<size_t> vec;
    vec.resize(length);
    std::iota(vec.begin(), vec.end(), 0);

    std::shuffle(vec.begin(), vec.end(),
                 std::default_random_engine(
                     std::chrono::high_resolution_clock::now().time_since_epoch().count()
                     )
                 );
    return vec;
}

#endif // TESTDATAGENERATORS_HPP
