#include <iostream>
#include <string>

#include "PoolAllocator.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"
#include "TreapSet.hpp"
#include "TreapSetTest.hpp"

using namespace promote;

//static std::string testInit();
static std::string testAdd();
//static std::string testContains();

static double testPerfAddSequential(bool pooled);
static double testPerfAddRandom(bool pooled);

void testing::perfTestTreapSet()
{
    std::cout << "\n\033[1m[ TreapSet ]\033[0m\n";
    testing::printSpeedResult("add (sequential,dynamic)", testPerfAddSequential(false));
    testing::printSpeedResult("add (sequential,pooled)", testPerfAddSequential(true));
    testing::printSpeedResult("add (random,dynamic)", testPerfAddRandom(false));
    testing::printSpeedResult("add (random,pooled)", testPerfAddRandom(true));
}

void testing::unitTestTreapSet()
{
    std::cout << "\n\033[1m[ TreapSet ]\033[0m\n";
    testing::printUnitResult("add", testAdd());
}

static std::string testAdd()
{
    TreapSet<std::size_t> set;
    for(std::size_t ii = 1; ii < 1000; ++ii) {
        if( !set.add(ii) ) {
            return "Failed to add new item.";
        } else if( set.size() != ii ) {
            return "Size not updated after add.";
        }
        for(std::size_t jj = 1; jj <= ii; ++jj) {
            if( set.add(jj) ) {
                return "Failed to reject duplicate.";
            } else if( set.size() != ii ) {
                return "Size changed after attempted duplicate add.";
            }
        }
    }
    return "";
}

static double testPerfAddSequential(bool pooled)
{
    StopWatch stopWatch;
    double total(0);

    if( !pooled ) {
        for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
            TreapSet<std::size_t> set;
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(count);
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        }
        return testing::calcNanosPer(total);
    } else {
        for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
            PoolAllocator allocator(TreapSet<std::size_t>::ALLOC_SIZE, testing::STANDARD_SPEED_COUNT);
            TreapSet<std::size_t> set(allocator);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(count);
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        }
        return testing::calcNanosPer(total);
    }
}   

static double testPerfAddRandom(bool pooled)
{
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::size_t randoms[testing::STANDARD_SPEED_COUNT];
        for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
            randoms[ii] = StandardRandomGenerator::instance().generate();
        }
        if( pooled ) {
            PoolAllocator allocator(TreapSet<std::size_t>::ALLOC_SIZE, testing::STANDARD_SPEED_COUNT);
            TreapSet<std::size_t> set(allocator);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(randoms[count]);
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        } else {
            TreapSet<std::size_t> set;
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(randoms[count]);
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        }
    }

    return testing::calcNanosPer(total);
}

