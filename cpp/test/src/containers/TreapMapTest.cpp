#include <iostream>

#include "PoolQueueAllocator.hpp"
#include "StandardHash.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"
#include "TreapMap.hpp"
#include "TreapMapTest.hpp"

using namespace promote;

static std::string testInit();
static std::string testInsert();
static std::string testContains();
//static std::string testFind();
static std::string testIterator();
//static std::string testRemove();
static double testSpeedAddSequential(bool pooled);
static double testSpeedAddRandom(bool pooled);

void testing::perfTestTreapMap()
{
    std::cout << "\n\033[1m[ TreapMap ]\033[0m\n";
    testing::printSpeedResult("add (sequential,dynamic)", testSpeedAddSequential(false));
    testing::printSpeedResult("add (sequential,pooled)", testSpeedAddSequential(true));
    testing::printSpeedResult("add (random,dynamic)", testSpeedAddRandom(false));
    testing::printSpeedResult("add (random,pooled)", testSpeedAddRandom(true));
}

void testing::unitTestTreapMap()
{
    std::cout << "\n\033[1m[ TreapMap ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("insert", testInsert());
    testing::printUnitResult("contains", testContains());
    testing::printUnitResult("iterator", testIterator());
}

static std::string testInit()
{
    TreapMap<int,double>* map;

    map = new TreapMap<int,double>();
    delete map;

    return "";
}

static std::string testInsert()
{
    TreapMap<std::size_t,double> map;
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
    TreapMap<std::size_t,double> map;
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

static std::string testIterator()
{
    for(std::size_t size = 1; size != 1000; ++size) {
        TreapMap<std::size_t,std::size_t> map;
        // Add
        for(std::size_t ii = 1; ii <= size; ++ii) {
            while(true) {
                std::size_t newKey(StandardRandomGenerator::instance().generate());
                if( !map.contains(newKey) ) {
                    map[newKey] = ii;
                    break;
                }
            }
        }

        // Iterate
        TreapMap<std::size_t,std::size_t>::Iterator iter;
        map.iterator(iter);
        std::size_t previousKey(0);
        std::size_t total(0);
        for(std::size_t ii = 1; ii <= size; ++ii) {
            if( !iter.hasNext() ) {
                return "HasNext failed when there should be entries left.";
            }
            TreapMapEntry<std::size_t,std::size_t> const& entry(iter.next());
            if( (ii > 1) and (entry.key() < previousKey) ) {
                return "Keys out of order in iterator.";
            }
            total += entry.value();
        }
        if( iter.hasNext() ) {
            return "HasNext returned true when there should be no entries left.";
        } else if( total != (size*(size+1))/2 ) {
            return "Values returned not correct.";
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
            TreapMap<std::size_t,std::size_t> map;
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
            PoolQueueAllocator<true,false> allocator(TreapMap<std::size_t,std::size_t>::ALLOC_SIZE, testing::STANDARD_SPEED_COUNT);
            TreapMap<std::size_t,std::size_t> map(allocator);
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
            PoolQueueAllocator<true,false> allocator(TreapMap<std::size_t,std::size_t>::ALLOC_SIZE, testing::STANDARD_SPEED_COUNT);
            TreapMap<std::size_t,std::size_t> map(allocator);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                map[randoms[count]] = count;
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        } else {
            TreapMap<std::size_t,std::size_t> map;
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

