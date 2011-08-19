#include "ArraySetTest.hpp"
#include "BinaryHeapTest.hpp"
#include "BitVectorTest.hpp"
#include "CircularlyLinkedListTest.hpp"
#include "DoublyLinkedListTest.hpp"
#include "HashMapTest.hpp"
#include "HashSetTest.hpp"
#include "IndexMapTest.hpp"
#include "QueueTest.hpp"
#include "SinglyLinkedListTest.hpp"
#include "SkipListMapTest.hpp"
#include "StackTest.hpp"
#include "STLContainerSpeedTest.hpp"
//#include "TreapMapTest.hpp"
#include "TreapSetTest.hpp"

#include <iostream>
#include <stdlib.h>
#include <ctime>

int main(void)
{
    srand(time(0));
    promote::testing::perfTestSTLContainers();
    promote::testing::perfTestArraySet();
    promote::testing::perfTestBinaryHeap();
    promote::testing::perfTestBitVector();
    promote::testing::perfTestCircularlyLinkedList();
    promote::testing::perfTestDoublyLinkedList();
    promote::testing::perfTestHashMap();
    promote::testing::perfTestHashSet();
    promote::testing::perfTestIndexMap();
    promote::testing::perfTestQueue();
    promote::testing::perfTestSinglyLinkedList();
    promote::testing::perfTestSkipListMap();
    promote::testing::perfTestStack();
   // promote::testing::perfTestTreapMap();
    promote::testing::perfTestTreapSet();
    return 0;
}

