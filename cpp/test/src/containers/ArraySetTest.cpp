#include <iostream>

#include "ArraySet.hpp"
#include "ArraySetTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

template <bool COPYSAFE> static std::string testInit();
template <bool COPYSAFE> static std::string testAdd();
template <bool COPYSAFE> static std::string testContains();
template <bool COPYSAFE> static std::string testRemove();
template <bool COPYSAFE> static std::string testIterator();
template <bool COPYSAFE> static double testPerfAdd();
template <bool COPYSAFE> static double testPerfGet();
template <bool COPYSAFE> static double testPerfIterator();
template <bool COPYSAFE> static double testPerfRemove();

void testing::perfTestArraySet()
{
    std::cout << "\n\033[1m[ ArraySet ]\033[0m\n";
    testing::printSpeedResult("add(copy=true)", testPerfAdd<true>());
    testing::printSpeedResult("add(copy=false)", testPerfAdd<false>());
    testing::printSpeedResult("get(copy=true)", testPerfGet<true>());
    testing::printSpeedResult("get(copy=false)", testPerfGet<false>());
    testing::printSpeedResult("iterator(copy=true)", testPerfIterator<true>());
    testing::printSpeedResult("iterator(copy=false)", testPerfIterator<false>());
    testing::printSpeedResult("remove(copy=true)", testPerfRemove<true>());
    testing::printSpeedResult("remove(copy=false)", testPerfRemove<false>());
}

void testing::unitTestArraySet()
{
    std::cout << "\n\033[1m[ ArraySet ]\033[0m\n";
    testing::printUnitResult("init(copy=true)", testInit<true>());
    testing::printUnitResult("init(copy=false)", testInit<false>());
    testing::printUnitResult("add(copy=true)", testAdd<true>());
    testing::printUnitResult("add(copy=false)", testAdd<false>());
    testing::printUnitResult("contains(copy=true)", testContains<true>());
    testing::printUnitResult("contains(copy=false)", testContains<false>());
    testing::printUnitResult("remove(copy=true)", testRemove<true>());
    testing::printUnitResult("remove(copy=false)", testRemove<false>());
    testing::printUnitResult("iterator(copy=false)", testIterator<false>());
    testing::printUnitResult("iterator(copy=true)", testIterator<true>());
}

template <bool COPYSAFE>
static std::string testInit()
{
    ArraySet<int,COPYSAFE>* set = new ArraySet<int,COPYSAFE>();
    delete set;

    return "";
}

template <bool COPYSAFE>
static std::string testAdd()
{
    ArraySet<std::size_t,COPYSAFE> set(1);
    for(std::size_t ii = 1; ii != 100; ++ii) {
        if( !set.add(ii) ) {
            return "Failed to add new value.";
        } else if( set.size() != ii ) {
            return "Failed to increment size after add.";
        }
        for(std::size_t jj = 1; jj <= ii; ++jj) {
            if( set.add(jj) ) {
                return "Failed to reject duplicate add.";
            } else if( set.size() != ii ) {
                return "Size messed up after failed add.";
            }
        }
    }
    return "";
}

template <bool COPYSAFE>
static std::string testContains()
{
    ArraySet<std::size_t,COPYSAFE> set(1);
    for(std::size_t ii = 0; ii != 100; ++ii) {
        set.add(ii);
        for(std::size_t jj = 0; jj != 120; ++jj) {
            if( jj <= ii ) {
                if( !set.contains(jj) ) {
                    return "Failed to find previously added item.";
                }
            } else if( set.contains(jj) ) {
                return "Contains returned true for item not added.";
            }
        }
    }
    return "";
}

template <bool COPYSAFE>
static std::string testRemove()
{
    ArraySet<std::size_t,COPYSAFE> set(1);
    for(std::size_t ii = 1; ii != 101; ++ii) {
        set.add(ii);
    }
    for(std::size_t ii = 1; ii != 101; ++ii) {
        if( !set.remove(ii) ) {
            return "Failed to remove added item.";
        } else if( set.size() != 100 - ii ) {
            return "Size not correct after remove.";
        }
        for(std::size_t jj = 1; jj != ii; ++jj) {
            if( set.remove(jj) ) {
                return "Failed to reject remove of non-existent item.";
            } else if( set.size() != 100 - ii ) {
                return "Size not correct after failed remove.";
            }
        }
    }
    return "";
}

template <bool COPYSAFE>
static std::string testIterator()
{
    ArraySet<std::size_t,COPYSAFE> set(100);
    bool finds[100];
    for(std::size_t ii = 0; ii != 100; ++ii) {
        set.add(ii);
        finds[ii] = false;
    }

    /**
     * Iterator 
     **/
    typename ArraySet<std::size_t,COPYSAFE>::Iterator iter(set);
    for(std::size_t ii = 0; ii != 100; ++ii) {
        if( !iter.hasNext() ) {
            return "Iterator ended prematurely.";
        }
        std::size_t value(iter.next());
        if( value < 0 or value >= 100 ) {
            return "Iterator returned value not in set.";
        } else if( finds[value] ) {
            return "Iterator returned value already found.";
        }
        finds[value] = true;
    }
    if( iter.hasNext() ) {
        return "Iterator does not correctly report hitting end.";
    }
    // NOTE: this is redundant, but it just feels right
    for(std::size_t ii = 0; ii != 100; ++ii) {
        if( !finds[ii] ) {
            return "Iterator did not return all items in set.";
        }
    }

    /**
     * ConstIterator
     **/
    for(std::size_t ii = 0; ii != 100; ++ii) {
        finds[ii] = false;
    }
    typename ArraySet<std::size_t,COPYSAFE>::ConstIterator constIter(set);
    for(std::size_t ii = 0; ii != 100; ++ii) {
        if( !constIter.hasNext() ) {
            return "ConstIterator ended prematurely.";
        }
        std::size_t value(constIter.next());
        if( value < 0 or value >= 100 ) {
            return "ConstIterator returned value not in set.";
        } else if( finds[value] ) {
            return "ConstIterator returned value already found.";
        }
        finds[value] = true;
    }
    if( constIter.hasNext() ) {
        return "ConstIterator does not correctly report hitting end.";
    }
    // NOTE: this is redundant, but it just feels right
    for(std::size_t ii = 0; ii != 100; ++ii) {
        if( !finds[ii] ) {
            return "ConstIterator did not return all items in set.";
        }
    }
    
    return "";
}

template <bool COPYSAFE>
static double testPerfAdd()
{
    const std::size_t iters(1000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter) {
        ArraySet<std::size_t,COPYSAFE> set(1);
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            set.add(count);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }

    return testing::calcNanosPer(total, iters);
}

template <bool COPYSAFE>
static double testPerfGet()
{
    const std::size_t iters(100000);
    StopWatch stopWatch;
    double total(0);

    ArraySet<std::size_t,COPYSAFE> set(testing::STANDARD_SPEED_COUNT);
    for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
        set.add(ii);
    }

    for(std::size_t iter = 0; iter != iters; ++iter) {
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            set[count];
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

template <bool COPYSAFE>
static double testPerfIterator()
{
    const std::size_t iters(100000);
    StopWatch stopWatch;
    double total(0);

    ArraySet<std::size_t,COPYSAFE> set(testing::STANDARD_SPEED_COUNT);
    for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
        set.add(ii);
    }

    for(std::size_t iter = 0; iter != iters; ++iter) {
        typename ArraySet<std::size_t,COPYSAFE>::Iterator iter(set);
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            iter.next();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

template <bool COPYSAFE>
static double testPerfRemove()
{
    const std::size_t iters(1000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter) {
        ArraySet<std::size_t, COPYSAFE> set(testing::STANDARD_SPEED_COUNT);
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            set.add(count);
        }
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            set.remove(count >> 1);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }

    return testing::calcNanosPer(total, iters);
}


