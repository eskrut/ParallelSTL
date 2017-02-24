#include <numeric>
#include <parallel/algorithm>

#include "../testdatagenerators.hpp"
#include "../stopwatch.h"

int main(int argc, char **argv)
{
    auto vec = makeShuffledVector();
    double baseDuration = 0;

    auto vecToSort = vec;
    {
        Stopwatch sw("plain sort");
        std::sort(std::begin(vecToSort), std::end(vecToSort));
        baseDuration = sw.duration();
    }
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed with plain sort");

    vecToSort = vec;
    {
        Stopwatch sw("parallel sort", baseDuration);
        std::__parallel::sort(std::begin(vecToSort), std::end(vecToSort));
    }
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed with parallel sort");

    return 0;
}
