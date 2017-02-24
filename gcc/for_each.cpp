#include <numeric>
#include <parallel/algorithm>

#include "../testdatagenerators.hpp"
#include "../stopwatch.h"

#include <list>
#include <thread>
#include <omp.h>

int main(int argc, char **argv)
{
    const size_t numParts = std::thread::hardware_concurrency() * 2 > 0 ?
                std::thread::hardware_concurrency() * 2 : 8;
    std::list<std::vector<size_t>> listOfVecs;
    listOfVecs.resize(numParts);
    std::generate(listOfVecs.begin(),
                  listOfVecs.end(),
                  [&](){ return makeShuffledVector(memoryToAlloc/numParts);}
    );
    double baseDuration = 0;
    
    auto list = listOfVecs;
    {
        Stopwatch sw("plain for, plain sort");
        for(auto &vecToSort : list)
            std::sort(std::begin(vecToSort), std::end(vecToSort));
        baseDuration = sw.duration();
    }
    for(const auto &vecToSort : list)
        if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
            throw std::runtime_error("Failed with plain for, plain sort");

    list = listOfVecs;
    {
        Stopwatch sw("plain for, parallel sort", baseDuration);
        for(auto &vecToSort : list)
            std::__parallel::sort(std::begin(vecToSort), std::end(vecToSort));
    }
    for(const auto &vecToSort : list)
        if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
            throw std::runtime_error("Failed plain for, parallel sort");

    list = listOfVecs;
    {
        Stopwatch sw("parallel for, plain sort", baseDuration);
        std::__parallel::for_each(std::begin(list), std::end(list),
                                  [](std::vector<size_t> &vecToSort){
            std::sort(std::begin(vecToSort), std::end(vecToSort));
        });
    }
    for(const auto &vecToSort : list)
        if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
            throw std::runtime_error("Failed parallel for, plain sort");

    list = listOfVecs;
    {
        Stopwatch sw("parallel for, parallel sort", baseDuration);
        std::__parallel::for_each(std::begin(list), std::end(list),
                                  [](std::vector<size_t> &vecToSort){
            std::__parallel::sort(std::begin(vecToSort), std::end(vecToSort));
        });
    }
    for(const auto &vecToSort : list)
        if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
            throw std::runtime_error("Failed parallel for, parallel sort");

    list = listOfVecs;
    {
        Stopwatch sw("openmp for, plain sort", baseDuration);
#pragma omp parallel
        {
#pragma omp single
            {
                for(auto it = list.begin(); it != list.end(); ++it)
#pragma omp task
                    sort(it->begin(), it->end());
            }
        }
    }
    for(const auto &vecToSort : list)
        if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
            throw std::runtime_error("Failed parallel for, parallel sort");

    return 0;
}
