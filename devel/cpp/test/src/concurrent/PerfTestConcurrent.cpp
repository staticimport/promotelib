#include "AtomicTest.hpp"
#include "AtomicCounterTest.hpp"
#include "FairSpinLockTest.hpp"
#include "MutexTest.hpp"
#include "ReadWriteMutexTest.hpp"
#include "ReentrantTest.hpp"
#include "SemaphoreTest.hpp"

#include <iostream>
#include <stdlib.h>
#include <ctime>

int main(void)
{
    srand(time(0));
    promote::testing::perfTestAtomic();
    promote::testing::perfTestAtomicCounter();
    promote::testing::perfTestFairSpinLock();
    promote::testing::perfTestMutex();
    promote::testing::perfTestReadWriteMutex();
    promote::testing::perfTestReentrant();
    promote::testing::perfTestSemaphore();
    return 0;
}

