#include "hpx/hpx_init.hpp"
#include "hpx/hpx.hpp"

#include "hpx/parallel/numeric.hpp"
#include "hpx/parallel/algorithm.hpp"

#include "../testdatagenerators.hpp"
#include "../stopwatch.h"

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
int hpx_main(){
    auto vec = makeShuffledVector();
    double baseDuration = 0;

    auto vecToSort = vec;
    {
        Stopwatch sw("plain sort");
        sort(std::begin(vecToSort), std::end(vecToSort));
        baseDuration = sw.duration();
    }
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed with plain sort");

    vecToSort = vec;
    {
        Stopwatch sw("seq sort", baseDuration);
        hpx::parallel::sort(hpx::parallel::seq, std::begin(vecToSort), std::end(vecToSort));
    }
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed with seq sort");

    vecToSort = vec;
    {
        Stopwatch sw("par sort", baseDuration);
        hpx::parallel::sort(hpx::parallel::par, std::begin(vecToSort), std::end(vecToSort));
    }
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed with par sort");

    vecToSort = vec;
    {
        Stopwatch sw("par_vec sort", baseDuration);
        hpx::parallel::sort(hpx::parallel::par_vec, std::begin(vecToSort), std::end(vecToSort));
    }
    if ( ! std::is_sorted(std::begin(vecToSort), std::end(vecToSort)) )
        throw std::runtime_error("Failed with par_vec sort");

    return hpx::finalize();
}
