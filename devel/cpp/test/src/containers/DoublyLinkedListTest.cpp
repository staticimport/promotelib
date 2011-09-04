#include <iostream>

#include "DoublyLinkedList.hpp"
#include "DoublyLinkedListTest.hpp"
#include "PoolAllocator.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testInit();
static std::string testPushBack(DoublyLinkedList<int>* sll, std::string prefix);
static std::string testPushBack();
static std::string testPushFront(DoublyLinkedList<int>* sll, std::string prefix);
static std::string testPushFront();
static std::string testInsert(DoublyLinkedList<int>* sll, std::string prefix);
static std::string testInsert();
static std::string testRemoveFront(DoublyLinkedList<int>* sll, std::string prefix);
static std::string testRemoveFront();
static std::string testRemove(DoublyLinkedList<int>* sll, std::string prefix);
static std::string testRemove();

static double testPerfPushBack(bool pooled);

void testing::perfTestDoublyLinkedList()
{
    std::cout << "\n\033[1m[ DoublyLinkedList ]\033[0m\n";
    testing::printSpeedResult("pushBack (dynamic)", testPerfPushBack(false));
    testing::printSpeedResult("pushBack (pooled)", testPerfPushBack(true));
}

void testing::unitTestDoublyLinkedList()
{
    std::cout << "\n\033[1m[ DoublyLinkedList ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("pushBack", testPushBack());
    testing::printUnitResult("pushFront", testPushFront());
    testing::printUnitResult("insert", testInsert());
    testing::printUnitResult("removeFront", testRemoveFront());
    testing::printUnitResult("remove", testRemove());
}

static std::string testInit()
{
    DoublyLinkedList<int>* sll;

    sll = new DoublyLinkedList<int>();
    delete sll;

    return "";
}

static std::string testPushBack(DoublyLinkedList<int>* sll, std::string prefix)
{
    for(int ii = 0; ii != 100; ++ii) {
        sll->pushBack(ii);
        if( sll->size() != static_cast<unsigned>(ii + 1) ) {
            return prefix + "Size not properly updated after pushBack.";
        }
        if( sll->front() != 0 )
            return prefix + "GetFront failed.";
        else if( sll->back() != ii )
            return prefix + "GetBack failed.";
        for(int jj = 0; jj <= ii; ++jj) {
            if( (*sll)[jj] != jj )
                return prefix + "Order of values not as expected after pushBack.";
        }
    }
    return "";
}

static std::string testPushBack()
{
    PoolAllocator allocator(DoublyLinkedList<int>::ALLOC_SIZE);
    DoublyLinkedList<int>* sll;
    std::string result;

    sll = new DoublyLinkedList<int>();
    result = testPushBack(sll, "[dynamic]");
    delete sll;
    
    if( result != "" )
        return result;

    sll = new DoublyLinkedList<int>(allocator);
    result = testPushBack(sll, "[pooled]");
    delete sll;

    return result;
}

static std::string testPushFront(DoublyLinkedList<int>* sll, std::string prefix)
{
    for(int ii = 0; ii != 100; ++ii) {
        sll->pushFront(ii);
        if( sll->size() != static_cast<unsigned>(ii + 1) ) {
            return prefix + "Size not properly updated after pushFront.";
        }
        if( sll->front() != ii )
            return prefix + "GetFront failed.";
        else if( sll->back() != 0 )
            return prefix + "GetBack failed.";
        for(int jj = 0; jj <= ii; ++jj) {
            if( (*sll)[jj] != ii - jj )
                return prefix + "Order of values not as expected after pushFront.";
        }
    }
    return "";
}

static std::string testPushFront()
{
    PoolAllocator allocator(DoublyLinkedList<int>::ALLOC_SIZE);
    DoublyLinkedList<int>* sll;
    std::string result;

    sll = new DoublyLinkedList<int>();
    result = testPushFront(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new DoublyLinkedList<int>(allocator);
    result = testPushFront(sll, "[pooled]");
    delete sll;

    return "";
}

static std::string testInsert(DoublyLinkedList<int>* sll, std::string prefix)
{
    for(int ii = 0; ii != 100; ++ii) {
        sll->insert(ii, 0);
        sll->insert(ii, sll->size() >> 1);
        sll->insert(ii, sll->size());
    }
    return "";
}

static std::string testInsert()
{
    PoolAllocator allocator(DoublyLinkedList<int>::ALLOC_SIZE);
    DoublyLinkedList<int>* sll;
    std::string result;

    sll = new DoublyLinkedList<int>();
    result = testInsert(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new DoublyLinkedList<int>(allocator);
    result = testInsert(sll, "[pooled]");
    delete sll;

    return "";
}

static std::string testRemoveFront(DoublyLinkedList<int>* sll, std::string prefix)
{
    int x;
    for(int ii = 0; ii != 100; ++ii)
        sll->pushBack(ii);
    for(int ii = 0; ii != 100; ++ii) {
        sll->removeFront(&x);
        if( sll->size() != static_cast<std::size_t>(99 - ii) )
            return "Failed to update size on removeFront";
        else if( x != ii )
            return "Failed to return correct value on removeFront";
        for(int jj = 0; jj != 99 - ii; ++jj) {
            if( (*sll)[jj] != jj + ii + 1 )
                return "Failed to return correct value on sequence of gets after removeFront.";
        }
    }
    return "";
}

static std::string testRemoveFront()
{
    PoolAllocator allocator(DoublyLinkedList<int>::ALLOC_SIZE);
    DoublyLinkedList<int>* sll;
    std::string result;

    sll = new DoublyLinkedList<int>();
    result = testRemoveFront(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new DoublyLinkedList<int>(allocator);
    result = testRemoveFront(sll, "[pooled]");
    delete sll;

    return "";
}

static std::string testRemove(DoublyLinkedList<int>* sll, std::string prefix)
{
    int x;
    for(int ii = 0; ii != 100; ++ii)
        sll->pushBack(ii);
    for(int ii = 99; ii != -1; --ii) {
        sll->remove(ii,&x);
        if( sll->size() != static_cast<std::size_t>(ii) )
            return "Size not updated on remove (tail).";
        else if( x != ii )
            return "Returned value not correct on remove (tail).";
        for(int jj = 0; jj != ii; ++jj) {
            if( (*sll)[jj] != jj )
                return "Sequence of gets not correct after remove (tail).";
        }
    }

    for(int ii = 0; ii != 100; ++ii)
        sll->pushBack(ii);
    for(int ii = 98; ii != -1; --ii) {
        sll->remove(ii,&x);
        if( sll->size() != static_cast<std::size_t>(ii + 1) )
            return "Size not updated on remove.";
        else if( x != ii )
            return "Returned value not correct on remove.";
        else if( sll->back() != 99 )
            return "Tail messed up after remove.";
        for(int jj = 0; jj != ii; ++jj) {
            if( (*sll)[jj] != jj )
                return "Sequence of gets not correct after remove.";
        }
    }
    return "";
}

static std::string testRemove()
{
    PoolAllocator allocator(DoublyLinkedList<int>::ALLOC_SIZE);
    DoublyLinkedList<int>* sll;
    std::string result;

    sll = new DoublyLinkedList<int>();
    result = testRemove(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new DoublyLinkedList<int>(allocator);
    result = testRemove(sll, "[pooled]");
    delete sll;

    return "";
}

static double testPerfPushBack(bool pooled)
{
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
        DoublyLinkedList<std::size_t>* sll;
        PoolAllocator allocator(DoublyLinkedList<std::size_t>::ALLOC_SIZE, testing::STANDARD_SPEED_COUNT);
        if( pooled ) {
            sll = new DoublyLinkedList<std::size_t>(allocator);
        } else {
            sll = new DoublyLinkedList<std::size_t>();
        }
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
            sll->pushBack(count);
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
        delete sll;
    }

    return testing::calcNanosPer(total);
}

