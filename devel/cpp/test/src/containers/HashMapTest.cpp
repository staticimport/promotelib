#include <iostream>

#include "HashMap.hpp"
#include "HashMapTest.hpp"
#include "PoolAllocator.hpp"
#include "StandardHash.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testInit();
static std::string testInsert();
static std::string testContains();
//static std::string testFind();
//static std::string testRemove();

static double testSpeedInsert(bool pooled, std::size_t initSize);

void testing::perfTestHashMap()
{
    std::cout << "\n\033[1m[ HashMap ]\033[0m\n";
    testing::printSpeedResult("insert (pooled,initCapacity=MAX)", testSpeedInsert(true,testing::STANDARD_SPEED_COUNT));
    testing::printSpeedResult("insert (pooled,initCapacity=1)", testSpeedInsert(true,1));
    testing::printSpeedResult("insert (!pooled,initCapacity=MAX)", testSpeedInsert(false,testing::STANDARD_SPEED_COUNT));
    testing::printSpeedResult("insert (!pooled,initCapacity=1)", testSpeedInsert(false,1));
}

void testing::unitTestHashMap()
{
    std::cout << "\n\033[1m[ HashMap ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("insert", testInsert());
    testing::printUnitResult("contains", testContains());
}

static std::string testInit()
{
    PoolAllocator allocator(sizeof(HashMap<int,std::string>::Entry));
    StandardInitializer<std::string> initializer;
    StandardHash<int> hash;
    HashMap<int,std::string>* map;

    map = new HashMap<int,std::string>();
    if( !map->empty() or map->size() != 0 ) {
        return "Size not initialized to 0 (a).";
    }
    delete map;

    map = new HashMap<int,std::string>(100);
    if( !map->empty() or map->size() != 0 ) {
        return "Size not initialized to 0 (b).";
    }
    delete map;

    map = new HashMap<int,std::string>(100, allocator);
    if( !map->empty() or map->size() != 0 ) {
        return "Size not initialized to 0 (c).";
    }
    delete map;

    map = new HashMap<int,std::string>(100, allocator, initializer);
    if( !map->empty() or map->size() != 0 ) {
        return "Size not initialized to 0 (d).";
    }
    delete map;

    map = new HashMap<int,std::string>(100, allocator, initializer, hash);
    if( !map->empty() or map->size() != 0 ) {
        return "Size not initialized to 0 (d).";
    }
    delete map;

    map = new HashMap<int,std::string>(10, allocator, initializer, hash, 0.5);
    if( !map->empty() or map->size() != 0 ) {
        return "Size not initialized to 0 (e).";
    } else if( map->loadFactor() != 0.5 ) {
        //printf("%f\n", map->loadFactor());
        std::cout << map->loadFactor() << std::endl;
        return "LoadFactor not properly initialized.";
    }
    delete map;

    return "";
}

static std::string testInsert()
{
    HashMap<std::size_t,double> map;
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
    HashMap<std::size_t,double> map;
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

static double testSpeedInsert(bool pooled, std::size_t initSize)
{
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        PoolAllocator* allocator(0);
        HashMap<std::size_t,std::size_t>* map;
        if( pooled ) {
            allocator = new PoolAllocator(HashMap<std::size_t,std::size_t>::ALLOC_SIZE, testing::STANDARD_SPEED_COUNT);
            map = new HashMap<std::size_t,std::size_t>(initSize, *allocator);
        } else {
            map = new HashMap<std::size_t,std::size_t>(initSize);
        }

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            (*map)[count] = count;
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();

        delete map;
        if( allocator ) {
            delete allocator;
        }
    }
    return testing::calcNanosPer(total);
}

