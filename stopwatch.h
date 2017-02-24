#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>

using hr_clock = std::chrono::high_resolution_clock;

struct Stopwatch {
    hr_clock::time_point start_;
    hr_clock::time_point stop_;
    std::string descr_;
    double duration_;
    double baseDuration_;
    bool isRunning_{false};
    void start() {
        start_ = hr_clock::now();
        isRunning_ = true;
    }
    void stop() {
        if(isRunning_) {
            stop_ = hr_clock::now();
            duration_ = (std::chrono::duration_cast<std::chrono::duration<double>>(stop_ - start_)).count();
            isRunning_ = false;
        }
    }
    double duration() const {
        if( isRunning_ )
            return (std::chrono::duration_cast<std::chrono::duration<double>>(hr_clock::now() - start_)).count();
        else
            return duration_;
    }
    Stopwatch(const std::string &descr, double baseDuration = -1) :
        descr_(descr),
        duration_(0),
        baseDuration_(baseDuration)
    {
        start();
    }
    ~Stopwatch()
    {
        stop();
        std::cout << std::setw(25) << std::left << descr_ << "\t" << std::setprecision(3) << duration_;
        if(baseDuration_ > 0) {
            std::cout << "\tspeedup " << std::setprecision(3) << baseDuration_/duration_;
        }
        std::cout << std::endl;
    }
};

#endif // STOPWATCH_H
