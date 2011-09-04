#include "PoolQueueAllocatorTest.hpp"

#include <cstdio>
#include <iostream>
#include <string>

#include "PoolQueueAllocator.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"
#include "TimeUtils.hpp"

using namespace promote;

static std::string testAlloc();
static std::string testRelease();

template <bool fixed, bool rwsafe>
static double testSpeedAlloc(const int size, const bool zero_out);

template <bool fixed, bool rwsafe>
static double testSpeedRelease(const int size);

void testing::testPoolQueueAllocator()
{
    std::cout << "\n\033[1m[ PoolQueueAllocator ]\033[0m\n";
    testing::printUnitResult("alloc", testAlloc());
    testing::printUnitResult("release", testRelease());

    testing::printSpeedResult("alloc (fixed,rwsafe,size=10,!zero_out)", testSpeedAlloc<true,true>(10,false));
    testing::printSpeedResult("alloc (fixed,rwsafe,size=10,zero_out)", testSpeedAlloc<true,true>(10,true));
    testing::printSpeedResult("alloc (fixed,rwsafe,size=100,!zero_out)", testSpeedAlloc<true,true>(100,false));
    testing::printSpeedResult("alloc (fixed,rwsafe,size=100,zero_out)", testSpeedAlloc<true,true>(100,true));
    testing::printSpeedResult("alloc (fixed,!rwsafe,size=10,!zero_out)", testSpeedAlloc<true,false>(10,false));
    testing::printSpeedResult("alloc (fixed,!rwsafe,size=10,zero_out)", testSpeedAlloc<true,false>(10,true));
    testing::printSpeedResult("alloc (fixed,!rwsafe,size=100,!zero_out)", testSpeedAlloc<true,false>(100,false));
    testing::printSpeedResult("alloc (fixed,!rwsafe,size=100,zero_out)", testSpeedAlloc<true,false>(100,true));
    testing::printSpeedResult("alloc (!fixed,rwsafe,size=10,!zero_out)", testSpeedAlloc<false,true>(10,false));
    testing::printSpeedResult("alloc (!fixed,rwsafe,size=10,zero_out)", testSpeedAlloc<false,true>(10,true));
    testing::printSpeedResult("alloc (!fixed,rwsafe,size=100,!zero_out)", testSpeedAlloc<false,true>(100,false));
    testing::printSpeedResult("alloc (!fixed,rwsafe,size=100,zero_out)", testSpeedAlloc<false,true>(100,true));
    testing::printSpeedResult("alloc (!fixed,!rwsafe,size=10,!zero_out)", testSpeedAlloc<false,false>(10,false));
    testing::printSpeedResult("alloc (!fixed,!rwsafe,size=10,zero_out)", testSpeedAlloc<false,false>(10,true));
    testing::printSpeedResult("alloc (!fixed,!rwsafe,size=100,!zero_out)", testSpeedAlloc<false,false>(100,false));
    testing::printSpeedResult("alloc (!fixed,!rwsafe,size=100,zero_out)", testSpeedAlloc<false,false>(100,true));

    testing::printSpeedResult("release (fixed,rwsafe,size=10)", testSpeedRelease<true,true>(10));
    testing::printSpeedResult("release (fixed,rwsafe,size=100)", testSpeedRelease<true,true>(100));
    testing::printSpeedResult("release (fixed,!rwsafe,size=10)", testSpeedRelease<true,false>(10));
    testing::printSpeedResult("release (fixed,!rwsafe,size=100)", testSpeedRelease<true,false>(100));
    testing::printSpeedResult("release (!fixed,rwsafe,size=10)", testSpeedRelease<false,true>(10));
    testing::printSpeedResult("release (!fixed,rwsafe,size=100)", testSpeedRelease<false,true>(100));
    testing::printSpeedResult("release (!fixed,!rwsafe,size=10)", testSpeedRelease<false,false>(10));
    testing::printSpeedResult("release (!fixed,!rwsafe,size=100)", testSpeedRelease<false,false>(100));
}

std::string testAlloc()
{
    PoolQueueAllocator<false,false> allocator(1000, 2);
    std::size_t const toAlloc(1000);
    char** alloced = new char*[toAlloc];

    for(int ii = 0; ii < 1000; ++ii) {
        alloced[ii] = static_cast<char*>(allocator.alloc(ii+1, true));
        if( alloced[ii] == 0 )
            return "Failed to allocate size " + (ii+1);
        //printf("ii=%d, alloced[ii]=%p\n", ii, alloced[ii]);
        for(int jj = 0; jj <= ii; ++jj) {
            if( alloced[ii][jj] != 0 )
                return "Failed to fully zero out alloced data.";
            else if( jj != ii and alloced[ii] == alloced[jj] ) {
                return "Returned space block twice!";
            }
        }
    }

    delete [] alloced;

    return "";
}

std::string testRelease()
{
    PoolQueueAllocator<false,false> allocator(5000, 2);

    for(int ii = 1; ii <= 5000; ++ii) {
        void* alloced = allocator.alloc(ii, true);
        if( alloced == 0 )
            return "Failed to allocate #" + ii;
        allocator.release(alloced);
    }

    return "";
}

template <bool fixed, bool rwsafe>
static double testSpeedAlloc(const int size, const bool zero_out)
{
    double total(0);
    StopWatch stopWatch;
    void** alloced = new void*[testing::STANDARD_SPEED_COUNT];

    for(unsigned iter = 0; iter < testing::STANDARD_SPEED_ITERS; ++iter) {
        PoolQueueAllocator<fixed,rwsafe> allocator(size, fixed ? testing::STANDARD_SPEED_COUNT : 1);
        stopWatch.start();
        for(unsigned count = 0; count < testing::STANDARD_SPEED_COUNT; ++count ) {
            alloced[count] = allocator.alloc(size, zero_out);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    delete [] alloced;

    return testing::calcNanosPer(total);
}

template <bool fixed, bool rwsafe>
static double testSpeedRelease(const int size)
{
    double total(0);
    StopWatch stopWatch;
    void** alloced = new void*[testing::STANDARD_SPEED_COUNT];

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        PoolQueueAllocator<fixed,rwsafe> allocator(size, fixed ? testing::STANDARD_SPEED_COUNT : 1);
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            alloced[count] = allocator.alloc(size, false);
        }
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            allocator.release(alloced[count]);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    delete [] alloced;
    
    return testing::calcNanosPer(total);
}

