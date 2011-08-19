#include <iostream>

#include "PoolAllocator.hpp"
#include "SinglyLinkedList.hpp"
#include "SinglyLinkedListTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testInit();
static std::string testPushBack(SinglyLinkedList<int>* sll, std::string prefix);
static std::string testPushBack();
static std::string testPushFront(SinglyLinkedList<int>* sll, std::string prefix);
static std::string testPushFront();
static std::string testInsert(SinglyLinkedList<int>* sll, std::string prefix);
static std::string testInsert();
static std::string testRemoveFront(SinglyLinkedList<int>* sll, std::string prefix);
static std::string testRemoveFront();
static std::string testRemove(SinglyLinkedList<int>* sll, std::string prefix);
static std::string testRemove();

static double testPerfPushBack(bool pooled);

void testing::perfTestSinglyLinkedList()
{
    std::cout << "\n\033[1m[ SinglyLinkedList ]\033[0m\n";
    testing::printSpeedResult("pushBack (dynamic)", testPerfPushBack(false));
    testing::printSpeedResult("pushBack (pooled)", testPerfPushBack(true));
}

void testing::unitTestSinglyLinkedList()
{
    std::cout << "\n\033[1m[ SinglyLinkedList ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("pushBack", testPushBack());
    testing::printUnitResult("pushFront", testPushFront());
    testing::printUnitResult("insert", testInsert());
    testing::printUnitResult("removeFront", testRemoveFront());
    testing::printUnitResult("remove", testRemove());
}

static std::string testInit()
{
    SinglyLinkedList<int>* sll;

    sll = new SinglyLinkedList<int>();
    delete sll;

    return "";
}

static std::string testPushBack(SinglyLinkedList<int>* sll, std::string prefix)
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
    PoolAllocator allocator(SinglyLinkedList<int>::ALLOC_SIZE);
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testPushBack(sll, "[dynamic]");
    delete sll;
    
    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(allocator);
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
    PoolAllocator allocator(SinglyLinkedList<int>::ALLOC_SIZE);
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testPushFront(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(allocator);
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
    PoolAllocator allocator(SinglyLinkedList<int>::ALLOC_SIZE);
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testInsert(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(allocator);
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
    PoolAllocator allocator(SinglyLinkedList<int>::ALLOC_SIZE);
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testRemoveFront(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(allocator);
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
    PoolAllocator allocator(SinglyLinkedList<int>::ALLOC_SIZE);
    SinglyLinkedList<int>* sll;
    std::string result;

    sll = new SinglyLinkedList<int>();
    result = testRemove(sll, "[dynamic]");
    delete sll;

    if( result != "" )
        return result;

    sll = new SinglyLinkedList<int>(allocator);
    result = testRemove(sll, "[pooled]");
    delete sll;

    return "";
}

static double testPerfPushBack(bool pooled)
{
    StopWatch stopWatch;

    if( pooled ) {
        for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
            PoolAllocator allocator(sizeof(std::size_t), testing::STANDARD_SPEED_COUNT);
            SinglyLinkedList<std::size_t> sll(allocator);
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                sll.pushBack(count);
            stopWatch.stop();
        }
    } else {
        for(std::size_t iter = 0; iter != testing::STANDARD_SPEED_ITERS; ++iter) {
            SinglyLinkedList<std::size_t> sll;
            stopWatch.start();
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                sll.pushBack(count);
            stopWatch.stop();
        }
    }

    return testing::calcNanosPer(getTimevalAsMicros(stopWatch.elapsed()));
}

