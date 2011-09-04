#include <iostream>

#include "BitVector.hpp"
#include "BitVectorTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testInit();
static std::string testSet();
static std::string testClear();
static std::string testSetBitsIterator();
static double testPerfSet(bool minCapacity);

void testing::perfTestBitVector()
{
    std::cout << "\n\033[1m[ BitVector ]\033[0m\n";
    testing::printSpeedResult("set(initCapacity=LO)", testPerfSet(true));
    testing::printSpeedResult("set(initCapacity=HI)", testPerfSet(false));
    /*testing::printSpeedResult("add(copy=true)", testPerfAdd<true>());
    testing::printSpeedResult("add(copy=false)", testPerfAdd<false>());
    testing::printSpeedResult("get(copy=true)", testPerfGet<true>());
    testing::printSpeedResult("get(copy=false)", testPerfGet<false>());
    testing::printSpeedResult("iterator(copy=true)", testPerfIterator<true>());
    testing::printSpeedResult("iterator(copy=false)", testPerfIterator<false>());
    testing::printSpeedResult("remove(copy=true)", testPerfRemove<true>());
    testing::printSpeedResult("remove(copy=false)", testPerfRemove<false>());*/
}

void testing::unitTestBitVector()
{
    std::cout << "\n\033[1m[ BitVector ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("set", testSet());
    testing::printUnitResult("clear", testClear());
    testing::printUnitResult("set bits iterator", testSetBitsIterator());
}

static std::string testInit()
{
    BitVector* bv;
    
    bv = new BitVector();
    delete bv;
    bv = new BitVector(1);
    delete bv;
    bv = new BitVector(10000);
    delete bv;

    return "";
}

static std::string testSet()
{
    std::size_t const end(10);
    BitVector* bv(new BitVector(1));
    for(std::size_t ii = 0; ii != end; ++ii) {
        if( ii & 1 ) {
            bv->set(ii);
        }
        for(std::size_t jj = 0; jj != end; ++jj) {
            if( (jj <= ii) and ((jj & 1) == 1) ) {
                if( !(*bv)[jj] ) {
                    return "Index should have been set but wasn't.";
                }
            } else if( (*bv)[jj] ) {
                return "Index was set but shouldn't have been.";
            }
        }
    }
    delete bv;

    return "";
}

static std::string testClear()
{
    std::size_t const end(1000);
    BitVector* bv(new BitVector(1));
    for(std::size_t ii = 0; ii != end; ++ii) {
        bv->set(ii);
    }
    for(std::size_t ii = 0; ii <= end + 10; ++ii) {
        if( ii & 1 ) {
            bv->clear(ii);
        }
        for(std::size_t jj = 0; jj <= end + 10; ++jj) {
            if( (jj >= end) or ((jj <= ii) and (jj & 1)) ) {
                if( (*bv)[jj] ) {
                    return "Index should have been cleared but wasn't.";
                }
            } else if( !(*bv)[jj] ) {
                return "Index was cleared but shouldn't have been.";
            }
        }
    }
    return "";
}

static std::string testSetBitsIterator()
{
    std::size_t const end(1000);
    BitVector* bv(new BitVector(1));
    for(std::size_t ii = 0; ii != end; ++ii) {
        if( ii % 3 == 0 ) {
            bv->set(ii);
        }
        std::size_t nextExpectedIndex(0);
        BitVector::SetBitsIterator iter(*bv);
        while( iter.hasNext() ) {
            std::size_t const index(iter.next());
            if( index != nextExpectedIndex ) {
                return "Wrong index returned while iterating.";
            }
            nextExpectedIndex += 3;
        }
        if( nextExpectedIndex <= ii ) {
            return "Iterator ended prematurely.";
        }
    }
    return "";
}

static double testPerfSet(bool const minCapacity)
{
    std::size_t const iters(1000);
    StopWatch stopWatch;

    for(std::size_t iter = 0; iter != iters; ++iter) {
        BitVector bv(minCapacity ? 1 : testing::STANDARD_SPEED_COUNT);
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            bv.set(count);
        }
        stopWatch.stop();
    }
    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()), iters * testing::STANDARD_SPEED_COUNT);
}

