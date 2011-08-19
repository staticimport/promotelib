#include "ArraySetTest.hpp"
#include "BinaryHeapTest.hpp"
#include "BitVectorTest.hpp"
#include "CircularlyLinkedListTest.hpp"
#include "DoublyLinkedListTest.hpp"
#include "HashMapTest.hpp"
#include "HashSetTest.hpp"
#include "IndexMapTest.hpp"
#include "LinkableTest.hpp"
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
    promote::testing::unitTestArraySet();
    promote::testing::unitTestBinaryHeap();
    promote::testing::unitTestBitVector();
    promote::testing::unitTestCircularlyLinkedList();
    promote::testing::unitTestDoublyLinkedList();
    promote::testing::unitTestHashMap();
    promote::testing::unitTestHashSet();
    promote::testing::unitTestIndexMap();
    promote::testing::unitTestLinkable();
    promote::testing::unitTestQueue();
    promote::testing::unitTestSinglyLinkedList();
    promote::testing::unitTestSkipListMap();
    promote::testing::unitTestStack();
    //promote::testing::unitTestTreapMap();
    promote::testing::unitTestTreapSet();
    return 0;
}

