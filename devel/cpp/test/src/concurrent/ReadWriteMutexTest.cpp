
#include <iostream>

#include "ReadWriteMutex.hpp"
#include "ReadWriteMutexTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static double testPerfLockUncontested(bool read);
static double testPerfUnlockUncontested(bool read);
static double testPerfTryLockUncontested(bool read, bool succeed);

void testing::perfTestReadWriteMutex()
{
    std::cout << "\n\033[1m[ ReadWriteMutex ]\033[0m\n";
    testing::printSpeedResult("read lock (uncontested)", testPerfLockUncontested(true));
    testing::printSpeedResult("read trylock (uncontested, success)",
                              testPerfTryLockUncontested(true, true));
    testing::printSpeedResult("read trylock (uncontested, fail)",
                              testPerfTryLockUncontested(true, false));
    testing::printSpeedResult("read unlock (uncontested)", testPerfUnlockUncontested(true));
    testing::printSpeedResult("write lock (uncontested)", testPerfLockUncontested(false));
    testing::printSpeedResult("write trylock (uncontested, success)",
                              testPerfTryLockUncontested(false, true));
    testing::printSpeedResult("write trylock (uncontested, fail)",
                              testPerfTryLockUncontested(false, false));
    testing::printSpeedResult("write unlock (uncontested)", testPerfUnlockUncontested(false));
}

void testing::unitTestReadWriteMutex()
{
    std::cout << "\n\033[1m[ ReadWriteMutex ]\033[0m\n";
}

static double testPerfLockUncontested(bool read)
{
    const std::size_t iters(1000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        ReadWriteMutex readWriteMutex[testing::STANDARD_SPEED_COUNT];
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            if( read ) {
                readWriteMutex[count].readLock();
            } else {
                readWriteMutex[count].writeLock();
            }
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            readWriteMutex[count].unlock();
        }
    }
    return testing::calcNanosPer(total, iters);
}

static double testPerfUnlockUncontested(bool read)
{
    const std::size_t iters(1000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)  {
        ReadWriteMutex readWriteMutex[testing::STANDARD_SPEED_COUNT];
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            if( read ) {
                readWriteMutex[count].readLock();
            } else {
                readWriteMutex[count].writeLock();
            }
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            readWriteMutex[count].unlock();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

static double testPerfTryLockUncontested(bool read, bool succeed)
{
    const std::size_t iters(1000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        ReadWriteMutex readWriteMutex[testing::STANDARD_SPEED_COUNT];
        if( !succeed ) {
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                readWriteMutex[count].writeLock();
            }
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            if( read ) {
                readWriteMutex[count].tryReadLock();
            } else {
                readWriteMutex[count].tryWriteLock();
            }
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            readWriteMutex[count].unlock();
        }
    }
    return testing::calcNanosPer(total, iters);
}


