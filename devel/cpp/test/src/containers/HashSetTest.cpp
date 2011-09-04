#include <iostream>

#include "HashSet.hpp"
#include "HashSetTest.hpp"
#include "PoolAllocator.hpp"
#include "StandardHash.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testInit();
static std::string testAdd();
static std::string testContains();
//static std::string testRemove();

static double testPerfAdd(bool pooled);

void testing::perfTestHashSet()
{
    std::cout << "\n\033[1m[ HashSet ]\033[0m\n";
    testing::printSpeedResult("add(!pooled)", testPerfAdd(false));
    testing::printSpeedResult("add(pooled)", testPerfAdd(true));
}

void testing::unitTestHashSet()
{
    std::cout << "\n\033[1m[ HashSet ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("add", testAdd());
    testing::printUnitResult("contains", testContains());
}

static std::string testInit()
{
    PoolAllocator allocator(sizeof(HashSet<int>::Entry));
    StandardHash<int> hash;
    HashSet<int>* set;

    set = new HashSet<int>();
    if( set->size() != 0 )
        return "Size not initialized to 0 (a).";
    delete set;

    set = new HashSet<int>(100);
    if( set->size() != 0 )
        return "Size not initialized to 0 (b).";
    delete set;

    set = new HashSet<int>(100, allocator);
    if( set->size() != 0 )
        return "Size not initialized to 0 (c).";
    delete set;

    set = new HashSet<int>(1000, allocator, hash);
    if( set->size() != 0 )
        return "Size not initialized to 0 (d).";
    delete set;

    set = new HashSet<int>(10, allocator, hash, 0.42);
    if( set->size() != 0 )
        return "Size not initialized to 0 (e).";
    else if( set->loadFactor() != 0.42f )
        return "LoadFactor not propertly initialized.";
    delete set;

    return "";
}

static std::string testAdd()
{
    HashSet<std::size_t> set(10,StandardAllocator::instance(),StandardHash<std::size_t>::instance(),2); // will ensure multiple entries in buckets

    for(std::size_t ii = 1; ii != 100; ++ii) {
        if( !set.add(ii) )
            return "Add failed for item not in set.";
        else if( set.size() != ii )
            return "Size not properly updated after add.";
    }

    for(std::size_t ii = 1; ii != 100; ++ii) {
        if( set.add(ii) )
            return "Add failed to return false for item already in set.";
        else if( set.size() != 99 )
            return "Size mistakenly changed on failed add.";
    }

    return "";
}

static std::string testContains()
{
    HashSet<std::size_t> set(10,StandardAllocator::instance(),StandardHash<std::size_t>::instance(),2); // will ensure multiple entries in buckets

    for(std::size_t ii = 1; ii != 100; ++ii) {
        set.add(ii);
        for(std::size_t jj = 1; jj <= ii; ++jj) {
            if( !set.contains(jj) )
                return "Contains failed for value in set.";
        }
        for(std::size_t jj = ii + 1; jj != 120; ++jj) {
            if( set.contains(jj) )
                return "Contains failed for value not in set.";
        }
    }

    return "";
}

/*static std::string testPushBack(SinglyLinkedList<int>* sll, std::string prefix)
{
    for(int ii = 0; ii != 100; ++ii) {
        if( !sll->pushBack(ii) ) {
            return prefix + "PushBack returned false unexpectedly.";
        } else if( sll->size() != static_cast<unsigned>(ii + 1) ) {
            return prefix + "Size not properly updated after pushBack.";
        }
        if( sll->getFront() != 0 )
            return prefix + "GetFront failed.";
        else if( sll->getBack() != ii )
            return prefix + "GetBack failed.";
        for(int jj = 0; jj <= ii; ++jj) {
            if( sll->get(jj) != jj )
                return prefix + "Order of values not as expected after pushBack.";
        }
    }
    return "";
}

static std::string testPushBack()
{
    using namespace testing;
    PoolAllocator allocator(sizeof(SinglyLinkedListNode<int>));
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testPushBack(sll, "[dynamic]");
    delete sll;
    
    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(&allocator);
    result = testPushBack(sll, "[pooled]");
    delete sll;

    return result;
}

static std::string testPushFront(SinglyLinkedList<int>* sll, std::string prefix)
{
    for(int ii = 0; ii != 100; ++ii) {
        if( !sll->pushFront(ii) ) {
            return prefix + "PushFront returned false unexpectedly.";
        } else if( sll->size() != static_cast<unsigned>(ii + 1) ) {
            return prefix + "Size not properly updated after pushFront.";
        }
        if( sll->getFront() != ii )
            return prefix + "GetFront failed.";
        else if( sll->getBack() != 0 )
            return prefix + "GetBack failed.";
        for(int jj = 0; jj <= ii; ++jj) {
            if( sll->get(jj) != ii - jj )
                return prefix + "Order of values not as expected after pushFront.";
        }
    }
    return "";
}

static std::string testPushFront()
{
    using namespace testing;
    PoolAllocator allocator(sizeof(SinglyLinkedListNode<int>));
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testPushFront(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(&allocator);
    result = testPushFront(sll, "[pooled]");
    delete sll;

    return "";
}

static std::string testInsert(SinglyLinkedList<int>* sll, std::string prefix)
{
    for(int ii = 0; ii != 100; ++ii) {
        if( !sll->insert(ii, 0) )
            return "Insert front failed.";
        else if( !sll->insert(ii, sll->size() >> 1) )
             return "Insert middle failed.";
        else if( !sll->insert(ii, sll->size()) )
             return "Insert end failed.";
    }
    return "";
}

static std::string testInsert()
{
    using namespace testing;
    PoolAllocator allocator(sizeof(SinglyLinkedListNode<int>));
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testInsert(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(&allocator);
    result = testInsert(sll, "[pooled]");
    delete sll;

    return "";
}

static std::string testRemoveFront(SinglyLinkedList<int>* sll, std::string prefix)
{
    int x;
    for(int ii = 0; ii != 100; ++ii)
        sll->pushBack(ii);
    for(int ii = 0; ii != 100; ++ii) {
        if( !sll->removeFront(&x) )
            return "Failed to removeFront";
        else if( sll->size() != static_cast<std::size_t>(99 - ii) )
            return "Failed to update size on removeFront";
        else if( x != ii )
            return "Failed to return correct value on removeFront";
        for(int jj = 0; jj != 99 - ii; ++jj) {
            if( sll->get(jj) != jj + ii + 1 )
                return "Failed to return correct value on sequence of gets after removeFront.";
        }
    }
    return "";
}

static std::string testRemoveFront()
{
    using namespace testing;
    PoolAllocator allocator(sizeof(SinglyLinkedListNode<int>));
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testRemoveFront(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(&allocator);
    result = testRemoveFront(sll, "[pooled]");
    delete sll;

    return "";
}

static std::string testRemove(SinglyLinkedList<int>* sll, std::string prefix)
{
    int x;
    for(int ii = 0; ii != 100; ++ii)
        sll->pushBack(ii);
    for(int ii = 99; ii != -1; --ii) {
        if( !sll->remove(ii,&x) )
            return "Remove (tail) failed.";
        else if( sll->size() != static_cast<std::size_t>(ii) )
            return "Size not updated on remove (tail).";
        else if( x != ii )
            return "Returned value not correct on remove (tail).";
        for(int jj = 0; jj != ii; ++jj) {
            if( sll->get(jj) != jj )
                return "Sequence of gets not correct after remove (tail).";
        }
    }

    for(int ii = 0; ii != 100; ++ii)
        sll->pushBack(ii);
    for(int ii = 98; ii != -1; --ii) {
        if( !sll->remove(ii,&x) )
            return "Remove failed.";
        else if( sll->size() != static_cast<std::size_t>(ii + 1) )
            return "Size not updated on remove.";
        else if( x != ii )
            return "Returned value not correct on remove.";
        else if( sll->getBack() != 99 )
            return "Tail messed up after remove.";
        for(int jj = 0; jj != ii; ++jj) {
            if( sll->get(jj) != jj )
                return "Sequence of gets not correct after remove.";
        }
    }
    return "";
}

static std::string testRemove()
{
    using namespace testing;
    PoolAllocator allocator(sizeof(SinglyLinkedListNode<int>));
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testRemove(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(&allocator);
    result = testRemove(sll, "[pooled]");
    delete sll;

    return "";
}

static double testPerfPushBack(bool pooled)
{
    using namespace testing;
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != STANDARD_SPEED_ITERS; ++iter) {
        PoolAllocator allocator(sizeof(std::size_t), STANDARD_SPEED_COUNT);
        SinglyLinkedList<std::size_t> sll(pooled ? &allocator : 0);
        stopWatch.start();
        for(std::size_t count = 0; count != STANDARD_SPEED_COUNT; ++count)
            sll.pushBack(count);
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }

    return total / STANDARD_SPEED_ITERS;
}*/

static double testPerfAdd(bool pooled)
{
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        if( pooled ) {
            PoolAllocator allocator(sizeof(HashSet<std::size_t>::Entry), testing::STANDARD_SPEED_COUNT);
            HashSet<std::size_t> set(testing::STANDARD_SPEED_COUNT, allocator);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(count);
            }
            stopWatch.stop();
        } else {
            HashSet<std::size_t> set(testing::STANDARD_SPEED_COUNT);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                set.add(count);
            }
            stopWatch.stop();
        }
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }

    return testing::calcNanosPer(total);
}

