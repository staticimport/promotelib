
#include <iostream>

#include "FairSpinLock.hpp"
#include "Reentrant.hpp"
#include "ReentrantTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

template <typename MUTEX> static double testPerfLock(bool const repeat);

//static double testPerfLockMutexRepeat();
//static double testPerfUnlockFinal();
//static double testPerfUnlockNotFinal();
//static double testPerfTryLockFirst(bool succeed);
//static double testPerfTryLockRepeat();

void testing::perfTestReentrant()
{
    std::cout << "\n\033[1m[ Reentrant ]\033[0m\n";
    testing::printSpeedResult("lock (FairSpinLock, first)", testPerfLock<FairSpinLock>(false));
    testing::printSpeedResult("lock (FairSpinLock, repeat)", testPerfLock<FairSpinLock>(true));
    testing::printSpeedResult("lock (Mutex, first)", testPerfLock<Mutex>(false));
    testing::printSpeedResult("lock (Mutex, repeat)", testPerfLock<Mutex>(true));
}

void testing::unitTestReentrant()
{
    std::cout << "\n\033[1m[ Reentrant ]\033[0m\n";
}

template <typename MUTEX>
static double testPerfLock(bool const repeat)
{
    const std::size_t iters(10000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        Reentrant<MUTEX> mutexes[testing::STANDARD_SPEED_COUNT];
        if( repeat ) {
            for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
                mutexes[ii].lock();
            }
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            mutexes[count].lock();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            mutexes[count].unlock();
            if( repeat ) {
                mutexes[count].unlock();
            }
        }
    }
    return testing::calcNanosPer(total, iters);
}
/*
static double testPerfUnlockUncontested()
{
    const std::size_t iters(10000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)  {
        Reentrant Reentrant[testing::STANDARD_SPEED_COUNT];
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            Reentrant[count].lock();
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            Reentrant[count].unlock();
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
        Reentrant Reentrant[testing::STANDARD_SPEED_COUNT];
        if( !succeed ) {
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                Reentrant[count].lock();
            }
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            Reentrant[count].trylock();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            Reentrant[count].unlock();
        }
    }
    return testing::calcNanosPer(total, iters);
}
*/

