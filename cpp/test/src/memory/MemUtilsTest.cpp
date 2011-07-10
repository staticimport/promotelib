#include "MemUtilsTest.hpp"

#include <cstring>
#include <iostream>
#include <string>

#include "MemUtils.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"
#include "TimeUtils.hpp"

using namespace promote;

static std::string testMemZero();
static double testSpeedMemSet(const std::size_t size);
static double testSpeedMemZero(const std::size_t size);

void testing::testMemUtils()
{
    std::cout << "\n\033[1m[ MemUtils ]\033[0m\n";
    testing::printUnitResult("memzero", testMemZero());
    testing::printSpeedResult("memset (size=1)", testSpeedMemSet(1));
    testing::printSpeedResult("memset (size=10)", testSpeedMemSet(10));
    testing::printSpeedResult("memset (size=100)", testSpeedMemSet(100));
    testing::printSpeedResult("memset (size=1000)", testSpeedMemSet(1000));
    testing::printSpeedResult("memzero (size=1)", testSpeedMemZero(1));
    testing::printSpeedResult("memzero (size=10)", testSpeedMemZero(10));
    testing::printSpeedResult("memzero (size=100)", testSpeedMemZero(100));
    testing::printSpeedResult("memzero (size=1000)", testSpeedMemZero(1000));
}

std::string testMemZero()
{
    for(std::size_t ii = 1; ii != 1000; ++ii) {
        char* mem = new char[ii+2];
        memset(mem, 'Z', ii+2);
        memzero(mem + 1, ii);
        for(std::size_t jj = 1; jj <= ii; ++jj) {
            if( mem[jj] != 0 ) {
                return "Failed to zero out all";
            }
        }
        if( mem[0] != 'Z' )
            return "Zeroed out character before intended region";
        else if( mem[ii + 1] != 'Z' )
            return "Zeroed out character after intended region";
        delete [] mem;
    }
    return "";
}

double testSpeedMemSet(const std::size_t size)
{
    double total(0);
    StopWatch stopWatch;
    char* mem = new char[size];

    for(unsigned iter = 0; iter < testing::STANDARD_SPEED_ITERS; ++iter) {
        stopWatch.start();
        for(unsigned count = 0; count < testing::STANDARD_SPEED_COUNT; ++count ) {
            memset(mem, 0, size);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    delete [] mem;

    return testing::calcNanosPer(total);
}

double testSpeedMemZero(const std::size_t size)
{
    double total(0);
    StopWatch stopWatch;
    char* mem = new char[size];

    for(unsigned iter = 0; iter < testing::STANDARD_SPEED_ITERS; ++iter) {
        stopWatch.start();
        for(unsigned count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            memzero(mem, size);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    delete [] mem;

    return testing::calcNanosPer(total);
}

