#include "hpx/hpx_init.hpp"
#include "hpx/hpx.hpp"

#include "hpx/parallel/numeric.hpp"
#include "hpx/parallel/algorithm.hpp"

#include "../testdatagenerators.hpp"
#include "../stopwatch.h"

#include <list>
#include <thread>

int main(int argc, char **argv)
{
    using namespace boost::program_options;
    options_description desc_commandline(
                "Usage: " HPX_APPLICATION_STRING " [options]");

    std::vector<std::string> const cfg = {
        "hpx.os_threads=all"
    };

    hpx::init(desc_commandline, argc, argv, cfg);

    return 0;
}
int hpx_main()
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
            hpx::parallel::sort(hpx::parallel::par,
                                std::begin(vecToSort),
                                std::end(vecToSort));
    }
    for(const auto &vecToSort : list)
        if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
            throw std::runtime_error("Failed plain for, parallel sort");

    list = listOfVecs;
    {
        Stopwatch sw("parallel for, plain sort", baseDuration);
        hpx::parallel::for_each(hpx::parallel::par,
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
        hpx::parallel::for_each(hpx::parallel::par,
                                std::begin(list), std::end(list),
                                [](std::vector<size_t> &vecToSort){
            hpx::parallel::sort(hpx::parallel::par,
                                std::begin(vecToSort),
                                std::end(vecToSort));
        });
    }
    for(const auto &vecToSort : list)
        if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
            throw std::runtime_error("Failed parallel for, parallel sort");

    return hpx::finalize();
}
