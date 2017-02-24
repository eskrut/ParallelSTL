
//This includes should be from ${T_LUTZ_ROOT}/include
#include <numeric>
#include <experimental/algorithm>

#include "../testdatagenerators.hpp"
#include "../stopwatch.h"

#include <list>
#include <thread>
#include <future>
#include <functional>

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
           sort(std::experimental::parallel::par, std::begin(vecToSort), std::end(vecToSort));
    }
    for(const auto &vecToSort : list)
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed plain for, parallel sort");

    list = listOfVecs;
    {
        Stopwatch sw("parallel for, plain sort", baseDuration);
        for_each(std::experimental::parallel::par,
                 std::begin(list), std::end(list),
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
        for_each(std::experimental::parallel::par,
                 std::begin(list), std::end(list),
                 [](std::vector<size_t> &vecToSort){
            sort(std::experimental::parallel::par, std::begin(vecToSort), std::end(vecToSort));
        });
    }
    for(const auto &vecToSort : list)
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed parallel for, parallel sort");

    //Compare fith std::async
    list = listOfVecs;
    {
        Stopwatch sw("for with async, plain sort", baseDuration);
        auto work = [](decltype(list.end()) begin, decltype(list.end()) end){
            for(auto it = begin; it != end; ++it)
            std::sort(it->begin(), it->end());
        };
        size_t hc = std::thread::hardware_concurrency();
        if(hc == 0) hc = 8;
        auto numThreads = std::min(list.size(), hc);
        auto chunkPerThread = list.size() / numThreads;
        auto threadBegin = list.begin();
        auto threadEnd = threadBegin;
        std::advance(threadEnd, chunkPerThread);
        std::list<std::future<void>> futures;
        for(size_t thId = 0; thId < numThreads - 1; ++thId){
            futures.push_back(std::async(std::launch::async,
                                         work, threadBegin, threadEnd));
            threadBegin = threadEnd;
            std::advance(threadEnd, chunkPerThread);
        }
        work(threadBegin, list.end());
        for(auto &f : futures) f.get();
    }
    for(const auto &vecToSort : list)
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed for with async, plain sort");

    
    return 0;
}
