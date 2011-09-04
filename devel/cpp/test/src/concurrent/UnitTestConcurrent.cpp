//#include "SharedVariableTest.hpp"
#include "VolatilePtrTest.hpp"

#include <stdlib.h>
#include <ctime>

int main(void)
{
    srand(time(0));
    //promote::testing::unitTestSharedVariable();
    promote::testing::unitTestVolatilePtr();
    return 0;
}

