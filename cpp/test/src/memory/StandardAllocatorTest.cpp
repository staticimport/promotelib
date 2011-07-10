#include "StandardAllocatorTest.hpp"

#include <iostream>
#include <string>

#include "StandardAllocator.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"
#include "TimeUtils.hpp"

using namespace promote;

static std::string testAlloc();
static std::string testRelease();
static double testSpeedAlloc(const int size, const bool zero_out);

void testing::testStandardAllocator()
{
    std::cout << "\n\033[1m[ StandardAllocator ]\033[0m\n";
    testing::printUnitResult("alloc", testAlloc());
    testing::printUnitResult("release", testRelease());
    testing::printSpeedResult("alloc (size=10,zero_out=false)", testSpeedAlloc(10,false));
    testing::printSpeedResult("alloc (size=10,zero_out=true)", testSpeedAlloc(10,true));
    testing::printSpeedResult("alloc (size=100,zero_out=false)", testSpeedAlloc(100,false));
    testing::printSpeedResult("alloc (size=100,zero_out=true)", testSpeedAlloc(100,true));
}

static std::string testAlloc()
{
    StandardAllocator allocator;

    for(int ii = 1; ii <= 1000; ++ii) {
        char* alloced = static_cast<char*>(allocator.alloc(ii, true));
        if( alloced == 0 )
            return "Failed to allocate size " + ii;
        for(int jj = 0; jj < ii; ++jj) {
            if( alloced[jj] != 0 )
                return "Failed to fully zero out alloced data.";
        }
    }

    return "";
}

static std::string testRelease()
{
    StandardAllocator allocator;

    for(int ii = 1; ii <= 5000; ++ii) {
        void* alloced = allocator.alloc(ii, true);
        if( alloced == 0 )
            return "Failed to allocate #" + ii;
        allocator.release(alloced);
    }

    return "";
}

static double testSpeedAlloc(const int size, const bool zero_out)
{
    double total(0);
    StandardAllocator allocator;
    StopWatch stopWatch;
    void** alloced = new void*[testing::STANDARD_SPEED_COUNT];

    for(unsigned iter = 0; iter < testing::STANDARD_SPEED_ITERS; ++iter) {
        stopWatch.start();
        for(unsigned count = 0; count < testing::STANDARD_SPEED_COUNT; ++count ) {
            //stopWatch.start();
            alloced[count] = allocator.alloc(size, zero_out);
            //stopWatch.stop();
            //allocator.release(alloced);
        }
        stopWatch.stop();
        for(unsigned count = 0; count < testing::STANDARD_SPEED_COUNT; ++count)
            allocator.release(alloced[count]);

        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    delete [] alloced;

    return testing::calcNanosPer(total);
}

