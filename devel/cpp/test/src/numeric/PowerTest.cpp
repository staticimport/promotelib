#include <iostream>

#include "Power.hpp"
#include "PowerTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testPower10s();
static std::string testLeastAbove();

void testing::testPower()
{
    std::cout << "\n\033[1m[ Power ]\033[0m\n";
    testing::printUnitResult("powers of 10", testPower10s());
    testing::printUnitResult("least above", testLeastAbove());
}

static std::string testPower10s()
{
    uint64_t expected(1);
    for(std::size_t index = 0; index != POWER_10s_LENGTH; ++index) {
        if( POWER_10s[index] != expected ) {
            return "POWER_10s array not correctly declared.";
        }
        expected *= 10;
    }
    return "";
}

static std::string testLeastAbove()
{
    if( leastPower2Above64(1,true) != 1 ) {
        return "Failed for 1,true.";
    } else if( leastPower2Above64(1,false) != 2 ) {
        return "Failed for 1,false.";
    }
    for(std::size_t power = 2; power != 62; ++power) {
        const uint64_t next(2ULL << power);
        const uint64_t previous(next >> 1);
        if( leastPower2Above64(previous,true) != previous ) {
            return "Failed for previous,true.";
        } else if( leastPower2Above64(previous,false) != next) {
            return "Failed for previous,false.";
        } else if( leastPower2Above64(previous+1,true) != next) {
            return "Failed for previous+1,true.";
        } else if( leastPower2Above64(next-1,false) != next) {
            return "Failed for next-1,false.";
        } else if( leastPower2Above64(next-1,true) != next) {
            return "Failed for next-1,true.";
        }
    }

    return "";
}

