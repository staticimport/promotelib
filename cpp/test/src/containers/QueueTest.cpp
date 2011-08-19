
#include <cstdio>
#include <iostream>

#include "Queue.hpp"
#include "QueueTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string testInit();
template <bool fixed, bool rwsafe> static std::string testWrite();
template <bool fixed, bool rwsafe> static std::string testRead();
template <bool fixed> static std::string testMultiThreaded();
static double testSpeedWrite(bool fixed, bool rwsafe);
static double testSpeedRead(bool fixed, bool rwsafe);
template <bool fixed,bool rwsafe> static double testSpeedMultiThreaded();

//static double testPerfPushBack(bool pooled);

void testing::perfTestQueue()
{
    std::cout << "\n\033[1m[ Queue ]\033[0m\n";
    testing::printSpeedResult("write(fixed,rwsafe)", testSpeedWrite(true,true));
    testing::printSpeedResult("write(fixed,!rwsafe)", testSpeedWrite(true,false));
    testing::printSpeedResult("write(!fixed,rwsafe)", testSpeedWrite(false,true));
    testing::printSpeedResult("write(!fixed,!rwsafe)", testSpeedWrite(false,false));
    testing::printSpeedResult("read(fixed,rwsafe)", testSpeedRead(true,true));
    testing::printSpeedResult("read(fixed,!rwsafe)", testSpeedRead(true,false));
    testing::printSpeedResult("read(!fixed,rwsafe)", testSpeedRead(false,true));
    testing::printSpeedResult("read(!fixed,!rwsafe)", testSpeedRead(false,false));
    //testing::printSpeedResult("multithreaded read/write (fixed,rwsafe)",
    //                          testSpeedMultiThreaded<true,true>());
}

void testing::unitTestQueue()
{
    std::cout << "\n\033[1m[ Queue ]\033[0m\n";
    testing::printUnitResult("init", testInit());
    testing::printUnitResult("write(fixed,rwsafe)", testWrite<true,true>());
    testing::printUnitResult("write(fixed,!rwsafe)", testWrite<true,false>());
    testing::printUnitResult("write(!fixed,rwsafe)", testWrite<false,true>());
    testing::printUnitResult("write(!fixed,!rwsafe)", testWrite<false,false>());
    testing::printUnitResult("read(fixed,rwsafe)", testRead<true,true>());
    testing::printUnitResult("read(fixed,!rwsafe)", testRead<true,false>());
    testing::printUnitResult("read(!fixed,rwsafe)", testRead<false,true>());
    testing::printUnitResult("read(!fixed,!rwsafe)", testRead<false,false>());
    testing::printUnitResult("multithreaded<fixed>", testMultiThreaded<true>());
    testing::printUnitResult("multithreaded<!fixed>", testMultiThreaded<false>());
}

static std::string testInit()
{
    Queue<int,true,false>* queue;

    queue = new Queue<int,true,false>(100);
    if( queue->size() != 0 )
        return "Size not 0 after init.";
    else if( queue->capacity() < 100 )
        return "Capacity not greater than specified min capacity.";
    delete queue;

    return "";
}

template <bool fixed, bool rwsafe>
static std::string testWrite()
{
    Queue<std::size_t,fixed,rwsafe>* queue;
    std::size_t capacity;

    queue = new Queue<std::size_t,fixed,rwsafe>(100);
    capacity = queue->capacity();
    for(std::size_t ii = 0; ii != std::min<std::size_t>(capacity,250); ++ii) {
        if( !queue->write(ii) )
            return "Failed to write when there should be room.";
        else if( queue->size() != ii + 1 ) {
            printf("Expected %d, got %d\n", (int)(ii+1), (int)queue->size());
            return "Size not correct after write.";
        }

    }
    if( fixed and queue->write(0) )
        return "Write returned true when queue should be full.";
    delete queue;

    queue = new Queue<std::size_t,fixed,rwsafe>(1000);
    capacity = queue->capacity();
    for(std::size_t ii = 0; ii != std::min<std::size_t>(capacity,2500); ++ii) {
        std::size_t* ptr = queue->beginWrite();
        if( ptr == 0 )
            return "BeginWrite returned NULL when there should be room.";
        else if( queue->size() != ii )
            return "Size shouldn't be incremented until after EndWrite.";
        *ptr = ii;
        queue->endWrite();
        if( queue->size() != ii + 1 )
            return "Size not correct after EndWrite.";
    }
    if( fixed and queue->beginWrite() != 0 )
        return "BeginWrite returned non-NULL pointer when queue should be full.";
    delete queue;

    return "";
}

template <bool fixed, bool rwsafe>
static std::string testRead()
{
    Queue<std::size_t,fixed,rwsafe>* queue;
    std::size_t capacity, value, count;

    queue = new Queue<std::size_t,fixed,rwsafe>(100);
    capacity = queue->capacity();
    count = std::min<std::size_t>(capacity, 5000);
    for(std::size_t ii = 0; ii != count; ++ii)
        queue->write(ii);
    for(std::size_t ii = 0; ii != count; ++ii) {
        if( !queue->read(value) )
            return "Read failed.";
        else if( value != ii )
            return "Read returned incorrect value.";
        else if( queue->size() != count - ii - 1 )
            return "Size not correct after read.";
    }
    if( fixed and queue->read(value) )
        return "Read supposedly succeeded when queue should be empty.";
    delete queue;

    queue = new Queue<std::size_t,fixed,rwsafe>(1000);
    capacity = queue->capacity();
    count = std::min<std::size_t>(capacity, 50000);
    for(std::size_t ii = 0; ii != count; ++ii)
        queue->write(ii);
    for(std::size_t ii = 0; ii != count; ++ii) {
        std::size_t* ptr = queue->beginRead();
        if( ptr == 0 )
            return "BeginRead returned NULL.";
        else if( *ptr != ii )
            return "BeginRead returned incorrect value.";
        else if( queue->size() != count - ii )
            return "Size shouldn't be decremented before EndRead.";
        queue->endRead();
        if( queue->size() != count - ii - 1 )
            return "Size not correct after EndRead.";
    }
    if( fixed and queue->beginRead() != 0 )
        return "BeginRead returned non-NULL when queue should be empty.";
    delete queue;

    return "";
}

static double testSpeedWrite(bool fixed, bool rwsafe)
{
    const std::size_t iters(100000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter) {
        if( fixed ) {
            if( rwsafe ) {
                Queue<std::size_t,true,true> queue(testing::STANDARD_SPEED_COUNT);
                stopWatch.start();
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.write(count);
                stopWatch.stop();
                total += getTimevalAsMicros(stopWatch.elapsed());
                stopWatch.reset();
            } else {
                Queue<std::size_t,true,false> queue(testing::STANDARD_SPEED_COUNT);
                stopWatch.start();
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.write(count);
                stopWatch.stop();
                total += getTimevalAsMicros(stopWatch.elapsed());
                stopWatch.reset();
            }
        } else {
            if( rwsafe ) {
                Queue<std::size_t,false,true> queue(testing::STANDARD_SPEED_COUNT);
                stopWatch.start();
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.write(count);
                stopWatch.stop();
                total += getTimevalAsMicros(stopWatch.elapsed());
                stopWatch.reset();
            } else {
                Queue<std::size_t,false,false> queue(testing::STANDARD_SPEED_COUNT);
                stopWatch.start();
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.write(count);
                stopWatch.stop();
                total += getTimevalAsMicros(stopWatch.elapsed());
                stopWatch.reset();
            }
        }
    }

    return testing::calcNanosPer(total, iters);
}

static double testSpeedRead(bool fixed, bool rwsafe)
{
    const std::size_t iters(100000);
    StopWatch stopWatch;
    double total(0);

    for(std::size_t iter = 0; iter != iters; ++iter) {
        if( fixed ) {
            if( rwsafe ) {
                Queue<std::size_t,true,true> queue(testing::STANDARD_SPEED_COUNT);
                std::size_t value;
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.write(count);
                stopWatch.start();
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.read(value);
                stopWatch.stop();
                total += getTimevalAsMicros(stopWatch.elapsed());
                stopWatch.reset();
            } else {
                Queue<std::size_t,true,false> queue(testing::STANDARD_SPEED_COUNT);
                std::size_t value;
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.write(count);
                stopWatch.start();
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.read(value);
                stopWatch.stop();
                total += getTimevalAsMicros(stopWatch.elapsed());
                stopWatch.reset();
            }
        } else {
            if( rwsafe ) {
                Queue<std::size_t,false,true> queue(testing::STANDARD_SPEED_COUNT);
                std::size_t value;
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.write(count);
                stopWatch.start();
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.read(value);
                stopWatch.stop();
                total += getTimevalAsMicros(stopWatch.elapsed());
                stopWatch.reset();
            } else {
                Queue<std::size_t,false,false> queue(testing::STANDARD_SPEED_COUNT);
                std::size_t value;
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.write(count);
                stopWatch.start();
                for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count)
                    queue.read(value);
                stopWatch.stop();
                total += getTimevalAsMicros(stopWatch.elapsed());
                stopWatch.reset();
            }
        }
    }

    return testing::calcNanosPer(total, iters);
}

template <bool fixed, bool rwsafe>
struct MultiThreadVars
{
    MultiThreadVars(std::size_t theCount) : queue(theCount), count(theCount), okay(true) { }

    Queue<std::size_t,fixed,rwsafe> queue;
    StopWatch stopWatch;
    std::size_t count;
    volatile bool okay;
};

template <bool rwsafe>
struct MultiThreadVars<false,rwsafe>
{
    MultiThreadVars(std::size_t theCount) : queue(1), count(theCount), okay(true) { }

    Queue<std::size_t,false,rwsafe> queue;
    StopWatch stopWatch;
    std::size_t count;
    volatile bool okay;
};

template <bool fixed>
static void* unitWriter(void* arg)
{
    MultiThreadVars<fixed,true>* vars(static_cast<MultiThreadVars<fixed,true>*>(arg));
    for(std::size_t ii = 0; ii != vars->count; ++ii) {
        //printf("Adding value: %llu\n", (unsigned long long)ii);
        while( vars->okay and !vars->queue.write(ii) ) ;
    }
    pthread_exit(0);
    return (void*)0;
}

template <bool fixed>
static void* unitReader(void* arg)
{
    MultiThreadVars<fixed,true>* vars(static_cast<MultiThreadVars<fixed,true>*>(arg));
    std::size_t value;
    for(std::size_t ii = 0; ii != vars->count; ++ii) {
        while( !vars->queue.read(value) ) ;
        if( value != ii ) {
            printf("Expected %d, got %d.", (int)ii, (int)value);
            //*((int*)0) = 3;
            vars->okay = false;
            break;
        }
    }
    pthread_exit(0);
    return (void*)0;
}

static void* urT(void* arg) { return unitReader<true>(arg); }
static void* urF(void* arg) { return unitReader<false>(arg); }
static void* uwT(void* arg) { return unitWriter<true>(arg); }
static void* uwF(void* arg) { return unitWriter<false>(arg); }

template <bool fixed>
static std::string testMultiThreaded()
{
    const std::size_t iters(100);
    const std::size_t count(100000);
    
    for(std::size_t iter = 0; iter != iters; ++iter) {
        MultiThreadVars<fixed,true> vars(count);
        pthread_t reader, writer;
        if( fixed ) {
            pthread_create(&reader, 0, &urT, &vars);
            pthread_create(&writer, 0, &uwT, &vars);
        } else {
            pthread_create(&reader, 0, &urF, &vars);
            pthread_create(&writer, 0, &uwF, &vars);
        }
        pthread_join(writer,0);
        pthread_join(reader,0);
        if( !vars.okay ) {
            return "Failed!";
        }
    }
    return "";
}

template <bool fixed, bool rwsafe>
static void* speedWriter(void* arg)
{
    MultiThreadVars<fixed,rwsafe>* vars(static_cast<MultiThreadVars<fixed,rwsafe>*>(arg));
    vars->stopWatch.start();
    std::size_t left(vars->count);
    while( left ) {
        if( vars->queue.write(left) ) {
            --left;
        }
    }
    pthread_exit(0);
    return (void*)0;
}

static void* swTT(void* arg) { return speedWriter<true,true>(arg); }

//static void speedWriter<true,true>(void* arg);

template <bool fixed, bool rwsafe>
static void* speedReader(void* arg)
{
    MultiThreadVars<fixed,rwsafe>* vars(static_cast<MultiThreadVars<fixed,rwsafe>*>(arg));
    std::size_t left(vars->count);
    std::size_t value;
    while( left ) {
        if( vars->queue.read(value) ) {
            --left;
        }
    }
    vars->stopWatch.stop();
    pthread_exit(0);
    return (void*)0;
}

static void* srTT(void* arg) { return speedReader<true,true>(arg); }

template <bool fixed, bool rwsafe>
static double testSpeedMultiThreaded()
{
    const std::size_t iters(1);
    const std::size_t count(100000);
    double total(0);
    
    for(std::size_t iter = 0; iter != iters; ++iter) {
        MultiThreadVars<fixed,rwsafe> vars(count);
        pthread_t reader, writer;
        pthread_create(&reader, 0, &srTT, &vars);
        pthread_create(&writer, 0, &swTT, &vars);
        pthread_join(reader,0);
        pthread_join(writer,0);
        total += getTimevalAsMicros(vars.stopWatch.elapsed());
    }
    return testing::calcNanosPer(total, iters, count);
}

