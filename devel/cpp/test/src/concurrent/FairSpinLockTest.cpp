
#include <iostream>

#include "FairSpinLock.hpp"
#include "FairSpinLockTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static double testPerfIsContested(bool contested);
static double testPerfLockUncontested();
static double testPerfUnlockUncontested();
static double testPerfTryLockUncontested(bool succeed);

void testing::perfTestFairSpinLock()
{
    std::cout << "\n\033[1m[ FairSpinLock ]\033[0m\n";
    testing::printSpeedResult("isContested (contested)", testPerfIsContested(true));
    testing::printSpeedResult("isContested (uncontested)", testPerfIsContested(false));
    testing::printSpeedResult("lock (uncontested)", testPerfLockUncontested());
    testing::printSpeedResult("unlock (uncontested)", testPerfUnlockUncontested());
    testing::printSpeedResult("trylock (uncontested, success)",
                              testPerfTryLockUncontested(true));
    testing::printSpeedResult("trylock (uncontested, fail)",
                              testPerfTryLockUncontested(false));
}

void testing::unitTestFairSpinLock()
{
    std::cout << "\n\033[1m[ FairSpinLock ]\033[0m\n";
}

static double testPerfIsContested(bool contested)
{
    std::size_t const iters(100000);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != iters; ++iter) {
        FairSpinLock lock;
        if( contested ) {
            lock.lock();
        }
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            lock.isContested();
        }
        stopWatch.stop();
    }
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()), iters);
}

static double testPerfLockUncontested()
{
    const std::size_t iters(10000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        FairSpinLock FairSpinLock[testing::STANDARD_SPEED_COUNT];
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            FairSpinLock[count].lock();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            FairSpinLock[count].unlock();
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
        FairSpinLock FairSpinLock[testing::STANDARD_SPEED_COUNT];
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            FairSpinLock[count].lock();
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            FairSpinLock[count].unlock();
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
        FairSpinLock FairSpinLock[testing::STANDARD_SPEED_COUNT];
        if( !succeed ) {
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                FairSpinLock[count].lock();
            }
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            FairSpinLock[count].trylock();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            FairSpinLock[count].unlock();
        }
    }
    return testing::calcNanosPer(total, iters);
}


