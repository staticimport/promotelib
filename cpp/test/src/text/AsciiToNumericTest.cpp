#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "AsciiToNumeric.hpp"
#include "AsciiToNumericTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static double testPerf_atoi(char const* string);
static double testPerf_strtof(char const* string);

//static std::string testIntegral();

template <typename T>
static std::string testA2Num(T const min, T const max, T const increment, T const allowedError, char const* format);

template <typename T>
static double testPerfA2Num(T const value, char const* format);

void testing::testAsciiToNumeric()
{
    std::cout << "\n\033[1m[ AsciiToNumeric ]\033[0m\n";
    testing::printUnitResult("char", testA2Num<char>(-127, 127, 1, 0, "%d"));
    testing::printUnitResult("int", testA2Num<int>(-100000, 100000, 1, 0, "%d"));
    testing::printUnitResult("long", testA2Num<long>(-100000, 100000, 1, 0, "%ld"));
    testing::printUnitResult("float", testA2Num<float>(-100, 100, 0.1, 0.0001, "%f"));
    testing::printUnitResult("double", testA2Num<double>(-1000, 1000, 0.1, 0.0001, "%f"));
    testing::printSpeedResult("atoi=10000", testPerf_atoi("10000"));
    testing::printSpeedResult("int=10000", testPerfA2Num<int>(10000, "%d"));
    testing::printSpeedResult("strtof=10000.12345", testPerf_strtof("10000.12345"));
    testing::printSpeedResult("float=10000.12345", testPerfA2Num<float>(10000.12345, "%f"));
    testing::printSpeedResult("double=10000.12345", testPerfA2Num<double>(10000.12345, "%f"));
}

/*static std::string testIntegral()
{
    char buf[64];
    for(int ii = -1000; ii != 1001; ++ii) {
        sprintf(buf, "%d", ii);
        if( parseIntegral<int>(buf, 63) != ii ) {
            return "Failed to parse encoded integral.";
        }
    }
    return "";
}*/

template <typename T>
static std::string testA2Num(T const min, T const max, T const increment, T const allowedError, char const* format)
{
    char buf[64];
    bool hitEnd(false);
    for(T ii = min; !hitEnd and ii <= max; ii += increment ) {
        hitEnd = (ii == max);
//        printf(format, ii);
//        printf("\n");
        buf[sprintf(buf, format, ii)] = 'Z';
        T output;
        if( !AsciiToNumeric<T>::parse(buf, 63, output) ) {
            return "Failed to parse encoded value.";
        } else if( output > ii + allowedError or output < ii - allowedError ) {
            return "Parse returned incorrect value.";
        }
    }
    return "";
}

static double testPerf_atoi(char const* string)
{
    std::size_t iters(10000000);
    StopWatch stopWatch;
    int foo(0);

    stopWatch.start();
    for(std::size_t iter = 0; iter != iters; ++iter) {
        foo = atoi(string);
    }
    stopWatch.stop();
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()), iters);
}

static double testPerf_strtof(char const* string)
{
    std::size_t iters(10000000);
    StopWatch stopWatch;

    stopWatch.start();
    for(std::size_t iter = 0; iter != iters; ++iter) {
        strtof(string, 0);
    }
    stopWatch.stop();
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()), iters);
}

template <typename T>
static double testPerfA2Num(T const value, char const* format)
{
    std::size_t iters(10000000);
    StopWatch stopWatch;
    T output;

    char buffer[128];
    buffer[sprintf(buffer, format, value)] = 'Z';

    stopWatch.start();
    for(std::size_t iter = 0; iter != iters; ++iter) {
        AsciiToNumeric<T>::parse(buffer, 127, output);
    }
    stopWatch.stop();
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()), iters);
}


