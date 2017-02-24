#include "hpx/hpx_init.hpp"
#include "hpx/hpx.hpp"

#include "hpx/parallel/numeric.hpp"
#include "hpx/parallel/algorithm.hpp"

#include "../testdatagenerators.hpp"
#include "../stopwatch.h"

#include <thread>
#include <chrono>

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

struct Processor
{
    void doWork(size_t id) {
        throw std::runtime_error("ò_ó from " + std::to_string(id));
    }
};

int hpx_main(){

    std::vector<Processor> processors;
    processors.resize(25);

    try {
        std::cout << "Plain for" << std::endl;
        std::for_each(processors.begin(), processors.end(),
                      [&](Processor &p){
            p.doWork(&p - processors.data());
        });
    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }

    try {
        std::cout << "Seq for" << std::endl;
        hpx::parallel::for_each(hpx::parallel::seq,
                                processors.begin(), processors.end(),
                                [&](Processor &p){
            p.doWork(&p - processors.data());
        });
    }
    catch(hpx::parallel::exception_list &list) {
        std::cout << "Exception list what: " << list.what() << std::endl;
        std::cout << "Total " << list.size() << " exceptions:" << std::endl;
        for(auto &e : list) {
            try{ boost::rethrow_exception(e); }
            catch(std::exception &e){
                std::cout << "\twhat: " << e.what() << std::endl;
            }
        }
    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }

    try {
        std::cout << "Par for" << std::endl;
        hpx::parallel::for_each(hpx::parallel::par, processors.begin(), processors.end(),
                                [&](Processor &p){
            p.doWork(&p - processors.data());
        });
    }
    catch(hpx::parallel::exception_list &list) {
        std::cout << "Exception list what: " << list.what() << std::endl;
        std::cout << "Total " << list.size() << " exceptions:" << std::endl;
        for(auto &e : list) {
            try{ boost::rethrow_exception(e); }
            catch(std::exception &e){
                std::cout << "\twhat: " << e.what() << std::endl;
            }
        }
    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }

    try {
        std::cout << "Par_vec for" << std::endl;
        hpx::parallel::for_each(hpx::parallel::par_vec, processors.begin(), processors.end(),
                                [&](Processor &p){
            p.doWork(&p - processors.data());
        });
    }
    catch(hpx::parallel::exception_list &list) {
        std::cout << "Exception list what: " << list.what() << std::endl;
        std::cout << "Total " << list.size() << " exceptions:" << std::endl;
        for(auto &e : list) {
            try{ boost::rethrow_exception(e); }
            catch(std::exception &e){
                std::cout << "\twhat: " << e.what() << std::endl;
            }
        }
    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }

    return hpx::finalize();
}
