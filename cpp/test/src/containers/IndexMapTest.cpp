
#include <iostream>
#include <set>

#include "IndexMap.hpp"
#include "IndexMapTest.hpp"
#include "StandardRandomGenerator.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testInit();
static std::string testModify();

static double testSpeedTailAdd(bool maxCapacity);
static double testSpeedLookup(double const positionFactor);

void testing::perfTestIndexMap()
{
    std::cout << "\n\033[1m[ IndexMap ]\033[0m\n";
    testing::printSpeedResult("lookup (front)", testSpeedLookup(0.0));
    testing::printSpeedResult("lookup (middle)", testSpeedLookup(0.5));
    testing::printSpeedResult("lookup (back)", testSpeedLookup(1.0));
    testing::printSpeedResult("tail add (initCapacity=1)", testSpeedTailAdd(1));
    testing::printSpeedResult("tail add (initCapacity=MAX)", testSpeedTailAdd(testing::STANDARD_SPEED_COUNT));
}

void testing::unitTestIndexMap()
{
    std::cout << "\n\033[1m[ IndexMap ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("set + contains", testModify());
}

static std::string testInit()
{
    IndexMap<int>* map1 = new IndexMap<int>();
    delete map1;

    IndexMap<std::string>* map2 = new IndexMap<std::string>();
    delete map2;

    map1 = new IndexMap<int>(100);
    delete map1;

    map2 = new IndexMap<std::string>(100);
    delete map2;

    map1 = new IndexMap<int>(100, StandardInitializer<int>::instance());
    delete map1;

    map2 = new IndexMap<std::string>(10, StandardInitializer<std::string>::instance());
    delete map2;

    return "";
}

static std::string testModify()
{
    std::size_t const count(1000);
    std::set<int> added;
    int* randoms = new int[count];
    for(std::size_t ii = 0; ii != count; ++ii) {
        randoms[ii] = static_cast<int>(StandardRandomGenerator::instance().generate() % (ii+5));
    }

    IndexMap<int> map;
    int max(0);
    for(std::size_t ii = 0; ii != count; ++ii) {
        //std::cout << "ii=" << ii << std::endl;
        int const next(randoms[ii]);
        max = std::max<int>(max, next);
        map[next] = 1;
        added.insert(next);
        for(int jj = 0; jj <= max; ++jj) {
            //std::cout << "jj=" << jj << std::endl;
            if( added.count(jj) != 0 ) {
                if( map[jj] == 0 ) {
                    return "[] didn't return value inserted.";
                }
            } else if( map[jj] != 0 ) {
                return "[] returned value never inserted.";
            }
        }
    }

    delete [] randoms;
    
    return "";
}

static double testSpeedTailAdd(bool maxCapacity)
{
    StopWatch stopWatch;
    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        IndexMap<std::size_t> map(maxCapacity ? testing::STANDARD_SPEED_COUNT : 1);

        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            map[count] = count;
        }
        stopWatch.stop();
    }
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()));
}

static double testSpeedLookup(double const positionFactor)
{
    StopWatch stopWatch;
    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        IndexMap<std::size_t> map(1);
        for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
            map[ii] = ii;
        }
        std::size_t index(static_cast<std::size_t>(positionFactor * testing::STANDARD_SPEED_COUNT));
        if( index >= testing::STANDARD_SPEED_COUNT ) {
            index = testing::STANDARD_SPEED_COUNT - 1;
        }
       
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            map[index] = count;
        }
        stopWatch.stop();
    }
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()));
}

