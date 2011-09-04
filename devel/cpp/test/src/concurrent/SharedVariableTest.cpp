
#include <iostream>

#include "PoolAllocator.hpp"
#include "SharedVariable.hpp"
#include "SharedVariableTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testInit();
static std::string testGetSet();
static std::string testCachedView();

void testing::perfTestSharedVariable()
{
    std::cout << "\n\033[1m[ SharedVariable ]\033[0m\n";
}

void testing::unitTestSharedVariable()
{
    std::cout << "\n\033[1m[ SharedVariable ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("get/set", testGetSet());
    testing::printUnitResult("cached view", testCachedView());
}

static std::string testInit()
{
    SharedVariable<int>* sv;
    sv = new SharedVariable<int>();
    delete sv;
    sv = new SharedVariable<int>(3);
    delete sv;

    SharedVariable<SharedVariable<int> >* sv2;
    sv2 = new SharedVariable<SharedVariable<int> >();
    delete sv2;

    return "";
}

static std::string testGetSet()
{
    SharedVariable<int> sv;
    for(int ii = 0; ii != 1000; ++ii) {
        sv.set(ii);
        if( *(sv.get()) != ii ) {
            return "Get failed after set.";
        }
    }
    return "";
}

static std::string testCachedView()
{
    SharedVariable<int> sv;
    SharedVariable<int>::CachedView* view(sv.createCachedView());
    view->set(0);
    for(int ii = 1; ii != 1000; ++ii) {
        sv.set(ii);
        if( view->getCached() != (ii-1) ) {
            return "Cached different after external set.";
        } else if( view->get() != ii ) {
            return "View returned wrong value on get.";
        } else if( view->getCached() != ii ) {
            return "Cached wrong after get.";
        }
    }
    sv.destroyCachedView(view);
    return "";
}

