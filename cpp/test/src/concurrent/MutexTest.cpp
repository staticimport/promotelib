
#include <iostream>

#include "Mutex.hpp"
#include "MutexTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static double testPerfLockUncontested();
static double testPerfUnlockUncontested();
static double testPerfTryLockUncontested(bool succeed);

void testing::perfTestMutex()
{
    std::cout << "\n\033[1m[ Mutex ]\033[0m\n";
    testing::printSpeedResult("lock (uncontested)", testPerfLockUncontested());
    testing::printSpeedResult("unlock (uncontested)", testPerfUnlockUncontested());
    testing::printSpeedResult("trylock (uncontested, success)",
                              testPerfTryLockUncontested(true));
    testing::printSpeedResult("trylock (uncontested, fail)",
                              testPerfTryLockUncontested(false));
}

void testing::unitTestMutex()
{
    std::cout << "\n\033[1m[ Mutex ]\033[0m\n";
}

static double testPerfLockUncontested()
{
    const std::size_t iters(10000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Mutex mutex[testing::STANDARD_SPEED_COUNT];
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            mutex[count].lock();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            mutex[count].unlock();
        }
    }
    return testing::calcNanosPer(total, iters);
}

static double testPerfUnlockUncontested()
{
    const std::size_t iters(10000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)  {
        Mutex mutex[testing::STANDARD_SPEED_COUNT];
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            mutex[count].lock();
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            mutex[count].unlock();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

static double testPerfTryLockUncontested(bool succeed)
{
    const std::size_t iters(10000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Mutex mutex[testing::STANDARD_SPEED_COUNT];
        if( !succeed ) {
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                mutex[count].lock();
            }
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            mutex[count].trylock();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            mutex[count].unlock();
        }
    }
    return testing::calcNanosPer(total, iters);
}


