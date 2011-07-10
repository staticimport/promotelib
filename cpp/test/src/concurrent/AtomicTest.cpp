
#include <iostream>

#include "Atomic.hpp"
#include "AtomicTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

template <typename T> double testPerfCompareAndSwap(bool success);
template <typename T> double testPerfAddAndFetch();
template <typename T> double testPerfFetch();

void testing::perfTestAtomic()
{
    std::cout << "\033[1m[ Atomic ]\033[0m\n";
    testing::printSpeedResult("compare-and-swap<int> (success)", 
                              testPerfCompareAndSwap<int>(true));
    testing::printSpeedResult("compare-and-swap<int> (fail)", 
                              testPerfCompareAndSwap<int>(false));
    testing::printSpeedResult("addAndFetch<int>", testPerfAddAndFetch<int>());
    testing::printSpeedResult("fetch<int>", testPerfFetch<int>());
}

template <typename T>
double testPerfCompareAndSwap(bool success)
{
    const std::size_t iters(30000);
    StopWatch stopWatch;
    double total(0);
    const T compare = success ? 0 : 1;

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Atomic<T> atomic;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            atomic.compareAndSwap(compare, compare);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

template <typename T>
double testPerfAddAndFetch()
{
    const std::size_t iters(30000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Atomic<T> atomic;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            atomic.addAndFetch(1);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

template <typename T>
double testPerfFetch()
{
    const std::size_t iters(30000);
    StopWatch stopWatch;
    double total(0);

    int foo(0);
    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Atomic<T> atomic;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            foo += atomic.fetch(); // To ensure fetch is actually called
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

