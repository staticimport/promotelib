#include "AnySizePoolAllocatorTest.hpp"
#include "CacheLineAllocatorTest.hpp"
#include "MemUtilsTest.hpp"
#include "PoolAllocatorTest.hpp"
#include "PoolQueueAllocatorTest.hpp"
#include "StandardAllocatorTest.hpp"

int main(void)
{
    promote::testing::testAnySizePoolAllocator();
    promote::testing::testCacheLineAllocator();
    promote::testing::testMemUtils();
    promote::testing::testPoolAllocator();
    promote::testing::testPoolQueueAllocator();
    promote::testing::testStandardAllocator();
    return 0;
}

