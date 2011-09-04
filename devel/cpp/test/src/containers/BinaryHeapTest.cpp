#include <iostream>
#include <string>

#include "BinaryHeap.hpp"
#include "BinaryHeapTest.hpp"
#include "PoolQueueAllocator.hpp"
#include "RandomGenerator.hpp"
#include "StandardRandomGenerator.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

//static std::string testInit();
static std::string testAdd();
static std::string testPop();
//static std::string testContains();

static double testPerfAddSequential(bool copySafe, bool pooled=false);
static double testPerfAddRandom(bool copySafe, bool pooled=false);

void testing::perfTestBinaryHeap()
{
    std::cout << "\n\033[1m[ BinaryHeap ]\033[0m\n";
    testing::printSpeedResult("add (sequential,copySafe)", testPerfAddSequential(true));
    testing::printSpeedResult("add (sequential,dynamic)", testPerfAddSequential(false,false));
    testing::printSpeedResult("add (sequential,pooled)", testPerfAddSequential(false,true));
    testing::printSpeedResult("add (random,copySafe)", testPerfAddRandom(true));
    testing::printSpeedResult("add (random,dynamic)", testPerfAddRandom(false,false));
    testing::printSpeedResult("add (random,pooled)", testPerfAddRandom(false,true));
}

void testing::unitTestBinaryHeap()
{
    std::cout << "\n\033[1m[ BinaryHeap ]\033[0m\n";
    testing::printUnitResult("add", testAdd());
    testing::printUnitResult("pop", testPop());
}

static std::string testAdd()
{
    BinaryHeap<std::size_t> set(1);
    for(std::size_t iter = 0; iter != 10; ++iter) {
        for(std::size_t ii = 1; ii <= 1000; ++ii) {
            set.add(ii);
            if( set.size() != (iter*1000)+ii ) {
                return "Size not updated after add.";
            }
        }
    }
    return "";
}

static std::string testPop()
{
    std::size_t const size(1000);

    BinaryHeap<std::size_t> set(1);
    for(std::size_t ii = size; ii; --ii) {
        set.add(ii);
    }
    for(std::size_t ii = 1; ii <= size; ++ii) {
        if( set.peek() != ii ) {
            return "Peek returned incorrect value.";
        }
        set.pop();
        if( set.size() != size - ii ) {
            return "Size not updated after pop.";
        }
    }
    return "";
}

template <typename T>
struct Setter {
    static void set(std::size_t& dest, T src) { dest = *src; }
};

template <>
struct Setter<std::size_t> {
    static void set(std::size_t& dest, std::size_t src) { dest = src; }
};

struct SomeClass {
    SomeClass(std::size_t const x_) : x(x_) { }

    bool operator==(SomeClass const& sc) const  { return x == sc.x; }
    bool operator!=(SomeClass const& sc) const  { return x != sc.x; }
    bool operator<(SomeClass const& sc) const   { return x < sc.x; }
    bool operator<=(SomeClass const& sc) const  { return x <= sc.x; }
    bool operator>(SomeClass const& sc) const   { return x > sc.x; }
    bool operator>=(SomeClass const& sc) const  { return x >= sc.x; }

    std::size_t const x;
};

static double testPerfAddSequential(bool primitive, bool pooled)
{
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        if( primitive ) {
            BinaryHeap<std::size_t,true,false> set(1);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(count);
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        } else if( pooled ) {
            PoolQueueAllocator<false,false> allocator(BinaryHeap<SomeClass,true,false>::ALLOC_SIZE, testing::STANDARD_SPEED_COUNT);
            BinaryHeap<SomeClass,true,false> set(allocator,1);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(SomeClass(count));
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        } else {
            BinaryHeap<SomeClass,true,false> set(1);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(SomeClass(count));
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        }
    }

    return testing::calcNanosPer(total);
}   

static double testPerfAddRandom(bool primitive, bool pooled)
{
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::size_t randoms[testing::STANDARD_SPEED_COUNT];
        for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
            randoms[ii] = StandardRandomGenerator::instance().generate();
        }
        if( primitive ) {
            BinaryHeap<std::size_t> set(1);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(randoms[count]);
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        } else if( pooled ) {
            PoolQueueAllocator<false,false> allocator(BinaryHeap<SomeClass,true,false>::ALLOC_SIZE, testing::STANDARD_SPEED_COUNT);
            BinaryHeap<SomeClass,true,false> set(allocator,1);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(SomeClass(randoms[count]));
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        } else {
            BinaryHeap<SomeClass,true,false> set(1);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(SomeClass(randoms[count]));
            }
            stopWatch.stop();
            total += getTimevalAsMicros(stopWatch.elapsed());
            stopWatch.reset();
        }
    }

    return testing::calcNanosPer(total);
}

