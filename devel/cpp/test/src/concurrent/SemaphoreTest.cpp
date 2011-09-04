
#include <iostream>

#include "Semaphore.hpp"
#include "SemaphoreTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static double testPerfSignalUncontested();
static double testPerfTryWaitUncontested(bool succeed);
static double testPerfWaitUncontested();

#ifndef __APPLE__
static double testPerfValueUncontested(bool noWait);
#endif

void testing::perfTestSemaphore()
{
    std::cout << "\n\033[1m[ Semaphore ]\033[0m\n";
    testing::printSpeedResult("signal (uncontested)", testPerfWaitUncontested());
    testing::printSpeedResult("trywait (uncontested, success)",
                              testPerfTryWaitUncontested(true));
    testing::printSpeedResult("trywait (uncontested, fail)",
                              testPerfTryWaitUncontested(false));
#ifndef __APPLE__
    testing::printSpeedResult("value (uncontested, available)",
                              testPerfValueUncontested(true));
    testing::printSpeedResult("value (uncontested, unavailable)",
                              testPerfValueUncontested(false));
#endif
    testing::printSpeedResult("wait (uncontested)", testPerfSignalUncontested());
}

void testing::unitTestSemaphore()
{
    std::cout << "\n\033[1m[ Semaphore ]\033[0m\n";
}

static double testPerfWaitUncontested()
{
    const std::size_t iters(100);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Semaphore semaphores[testing::STANDARD_SPEED_COUNT];
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            semaphores[count].wait();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            semaphores[count].signal();
        }
    }
    return testing::calcNanosPer(total, iters);
}

static double testPerfSignalUncontested()
{
    const std::size_t iters(100);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)  {
        Semaphore semaphores[testing::STANDARD_SPEED_COUNT];
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            semaphores[count].wait();
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            semaphores[count].signal();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

static double testPerfTryWaitUncontested(bool succeed)
{
    const std::size_t iters(100);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Semaphore semaphores[testing::STANDARD_SPEED_COUNT];
        if( !succeed ) {
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                semaphores[count].wait();
            }
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            semaphores[count].tryWait();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            semaphores[count].signal();
        }
    }
    return testing::calcNanosPer(total, iters);
}

#ifndef __APPLE__
static double testPerfValueUncontested(bool noWait)
{
    const std::size_t iters(10000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Semaphore semaphores[testing::STANDARD_SPEED_COUNT];
        if( !noWait ) {
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                semaphores[count].wait();
            }
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            semaphores[count].value();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        if( !noWait ) {
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                semaphores[count].signal();
            }
        }
    }
    return testing::calcNanosPer(total, iters);
}
#endif


