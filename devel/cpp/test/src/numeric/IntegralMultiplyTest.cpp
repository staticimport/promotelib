#include <iostream>

#include "IntegralMultiply.hpp"
#include "IntegralMultiplyTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static double testPerfStandardTimes1000();
static double testPerfTimes1000();

void testing::testIntegralMultiply()
{
    std::cout << "\n\033[1m[ IntegralMultiply ]\033[0m\n";
    testing::printSpeedResult("std multiply 1000", testPerfStandardTimes1000());
    testing::printSpeedResult("multiply 1000", testPerfTimes1000());
}

static double testPerfStandardTimes1000()
{
    std::size_t const iters(10000);
    std::size_t const count(1000000);
    StopWatch stopWatch;
    std::size_t ignored(0);

    for(std::size_t iter = 0; iter != iters; ++iter) {
        stopWatch.start();
        for(std::size_t c = 0; c != count; ++c) {
            ignored += iters * 1000;
        }
        stopWatch.stop();
    }
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()), iters, count);
}

static double testPerfTimes1000()
{
    std::size_t const iters(10000);
    std::size_t const count(1000000);
    StopWatch stopWatch;
    std::size_t ignored(0);

    for(std::size_t iter = 0; iter != iters; ++iter) {
        stopWatch.start();
        for(std::size_t c = 0; c != count; ++c) {
            ignored += IntegralMultiply<std::size_t,1000>::multiply(iters);
        }
        stopWatch.stop();
    }
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()), iters, count);
}

