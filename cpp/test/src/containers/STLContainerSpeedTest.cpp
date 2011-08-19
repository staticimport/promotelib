
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
//#include <unordered_map>
#include <vector>

#include "StandardRandomGenerator.hpp"
#include "STLContainerSpeedTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"
#include "TimeUtils.hpp"

using namespace promote;

static double testListPushBack();
static double testMapAddSequential();
static double testPriorityQueueAddSequential();
static double testPriorityQueueAddRandom();
static double testQueuePop();
static double testQueuePush();
static double testSetAddSequential();
static double testSetAddRandom();
static double testStackPush();
static double testVectorPushBack();
//static double testUnorderedMapAdd();

void testing::perfTestSTLContainers()
{
    std::cout << "\n\033[1m[ STL Containers ]\033[0m\n";
    testing::printSpeedResult("list.push_back", testListPushBack());
    testing::printSpeedResult("map.add (sequential)", testMapAddSequential());
    testing::printSpeedResult("priority_queue.push_back (sequential)", testPriorityQueueAddSequential());
    testing::printSpeedResult("priority_queue.push_back (random)", testPriorityQueueAddRandom());
    testing::printSpeedResult("queue.pop", testQueuePop());
    testing::printSpeedResult("queue.push", testQueuePush());
    testing::printSpeedResult("set.add (sequential)", testSetAddSequential());
    testing::printSpeedResult("set.add (random)", testSetAddRandom());
    testing::printSpeedResult("stack.push", testStackPush());
    testing::printSpeedResult("vector.push_back", testVectorPushBack());
    //testing::printSpeedResult("unordered_map.add", testUnorderedMapAdd());
}

struct FooBar {
    FooBar(std::size_t s_) : s(s_) { }
    FooBar(FooBar const& fb) : s(fb.s) { }

    bool operator==(FooBar const& fb) const { return s == fb.s; }
    bool operator!=(FooBar const& fb) const { return s != fb.s; }
    bool operator<(FooBar const& fb) const  { return s < fb.s; }
    bool operator<=(FooBar const& fb) const { return s <= fb.s; }
    bool operator>(FooBar const& fb) const  { return s > fb.s; }
    bool operator>=(FooBar const& fb) const { return s >= fb.s; }

    FooBar& operator=(FooBar const& fb) {
        if( this != &fb ) {
            s = fb.s;
        }
        return *this;
    }

    std::size_t s;
    double d;
    float f;
    std::string str;
};


static double testListPushBack()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::list<std::size_t> list;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
            list.push_back(count);
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }

    return testing::calcNanosPer(total);
}

static double testMapAddSequential()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::map<std::size_t,std::size_t> map;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            map[count] = count;
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total);
}

static double testPriorityQueueAddSequential()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::priority_queue<FooBar> pq;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            pq.push(FooBar(count));
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total);
}

static double testPriorityQueueAddRandom()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::size_t randoms[testing::STANDARD_SPEED_COUNT];
        for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
            randoms[ii] = StandardRandomGenerator::instance().generate();
        }
        std::priority_queue<FooBar> pq;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            pq.push(FooBar(randoms[count]));
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total);
}

static double testQueuePop()
{
    StopWatch stopWatch;
    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::queue<std::size_t> queue;
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            queue.push(count);
        }
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            queue.pop();
        }
        stopWatch.stop();
    }
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()));
}

static double testQueuePush()
{
    StopWatch stopWatch;
    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::queue<std::size_t> queue;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            queue.push(count);
        }
        stopWatch.stop();
    }
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()));
}

static double testSetAddSequential()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::set<std::size_t> set;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            set.insert(count);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total);
}

static double testSetAddRandom()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::size_t randoms[testing::STANDARD_SPEED_COUNT];
        for(std::size_t ii = 0; ii != testing::STANDARD_SPEED_COUNT; ++ii) {
            randoms[ii] = StandardRandomGenerator::instance().generate();
        }
        std::set<std::size_t> set;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            set.insert(randoms[count]);
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total);
}

static double testStackPush()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::stack<std::size_t> stack;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
            stack.push(count);
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }

    return testing::calcNanosPer(total);
}

/*static double testUnorderedMapAdd()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::unordered_map<std::size_t,std::size_t> map;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            map[count] = count;
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total);
}*/

static double testVectorPushBack()
{
    double total(0);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        std::vector<std::size_t> vector;
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
            vector.push_back(count);
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }

    return testing::calcNanosPer(total);
}

