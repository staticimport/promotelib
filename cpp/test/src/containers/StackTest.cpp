#include <iostream>

#include "Stack.hpp"
#include "StackTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static double testSpeedPush();

void testing::perfTestStack()
{
    std::cout << "\n\033[1m[ Stack ]\033[0m\n";
    testing::printSpeedResult("push", testSpeedPush());
}

void testing::unitTestStack()
{
    std::cout << "\n\033[1m[ Stack ]\033[0m\n";
}

static double testSpeedPush()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        Stack<std::size_t> stack(testing::STANDARD_SPEED_COUNT);
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
            stack.push(count);
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }

    return testing::calcNanosPer(total);
}

