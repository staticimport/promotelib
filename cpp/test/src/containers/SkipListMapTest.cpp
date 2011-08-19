#include <iostream>

#include "AnySizePoolAllocator.hpp"
#include "StandardHash.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"
#include "SkipListMap.hpp"
#include "SkipListMapTest.hpp"

using namespace promote;

static std::string testInit();
static std::string testInsert();
static std::string testContains();
//static std::string testFind();
//static std::string testRemove();
static double testSpeedAddSequential(bool pooled);
static double testSpeedAddRandom(bool pooled);

void testing::perfTestSkipListMap()
{
    std::cout << "\n\033[1m[ SkipListMap ]\033[0m\n";
    testing::printSpeedResult("add (sequential,dynamic)", testSpeedAddSequential(false));
    testing::printSpeedResult("add (sequential,pooled)", testSpeedAddSequential(true));
    testing::printSpeedResult("add (random,dynamic)", testSpeedAddRandom(false));
    testing::printSpeedResult("add (random,pooled)", testSpeedAddRandom(true));
}

void testing::unitTestSkipListMap()
{
    std::cout << "\n\033[1m[ SkipListMap ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("insert", testInsert());
    testing::printUnitResult("contains", testContains());
}

static std::string testInit()
{
    SkipListMap<int,double>* map;

    map = new SkipListMap<int,double>();
    delete map;

    return "";
}

static std::string testInsert()
{
    SkipListMap<std::size_t,double> map;
    for(std::size_t ii = 1; ii != 100; ++ii) {
        map[ii] = 0;
        if( map.size() != ii ) {
            return "Size incorrect after insert.";
        }
        for(std::size_t jj = ii; jj; --jj) {
            map[jj] = 1;
            if( map.size() != ii ) {
                return "Size incorrect after update.";
            }
        }
    }
    return "";
}

static std::string testContains()
{
    SkipListMap<std::size_t,double> map;
    for(std::size_t ii = 1; ii != 100; ++ii) {
        map[ii] = 0;
        for(std::size_t jj = ii; jj; --jj) {
            if( !map.contains(jj) ) {
                return "Contains failed on inserted item.";
            }
        }
        for(std::size_t jj = 101; jj != 150; ++jj) {
            if( map.contains(jj) ) {
                return "Contains failed on uninserted item.";
            }
        }
    }
    return "";
}

static double testSpeedAddSequential(bool pooled)
{
    StopWatch stopWatch;
    double total(0);

    if( !pooled ) {
        for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
            SkipListMap<std::size_t,std::size_t,false> map;
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                map[count] = count;
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        }
        return testing::calcNanosPer(total);
    } else {
        for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
            AnySizePoolAllocator allocator(testing::STANDARD_SPEED_COUNT); // TODO
            SkipListMap<std::size_t,std::size_t,false> map(allocator);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                map[count] = count;
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        }
        return testing::calcNanosPer(total);
    }
}

static double testSpeedAddRandom(bool pooled)
{
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::size_t randoms[testing::STANDARD_SPEED_COUNT];
        for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
            randoms[ii] = StandardRandomGenerator::instance().generate();
        }
        if( pooled ) {
            AnySizePoolAllocator allocator(testing::STANDARD_SPEED_COUNT); // TODO
            SkipListMap<std::size_t,std::size_t,false> map(allocator);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                map[randoms[count]] = count;
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        } else {
            SkipListMap<std::size_t,std::size_t,false> map;
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                map[randoms[count]] = count;
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        }
    }
    return testing::calcNanosPer(total);
}

