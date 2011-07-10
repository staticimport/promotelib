
#include <iostream>

#include "PoolAllocator.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"
#include "VolatilePtr.hpp"
#include "VolatilePtrTest.hpp"

using namespace promote;

static std::string testInit();

void testing::perfTestVolatilePtr()
{
    std::cout << "\n\033[1m[ VolatilePtr ]\033[0m\n";
}

void testing::unitTestVolatilePtr()
{
    std::cout << "\n\033[1m[ VolatilePtr ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    //testing::printUnitResult("get/set", testGetSet());
    //testing::printUnitResult("cached view", testCachedView());
}

static std::string testInit()
{
    VolatilePtr<int>* sv;
    sv = new VolatilePtr<int>();
    delete sv;

    VolatilePtr<VolatilePtr<int> >* sv2;
    sv2 = new VolatilePtr<VolatilePtr<int> >();
    delete sv2;

    return "";
}

