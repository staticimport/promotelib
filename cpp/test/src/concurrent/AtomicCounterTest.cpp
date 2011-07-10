
#include <iostream>
#include <pthread.h>

#include "AtomicCounter.hpp"
#include "AtomicCounterTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

template <typename T> double testPerfIncrement(unsigned threads, bool useThreadView);
template <typename T> double testPerfValue(unsigned threads);

void testing::perfTestAtomicCounter()
{
    std::cout << "\n\033[1m[ AtomicCounter ]\033[0m\n";
    testing::printSpeedResult("increment (1 thread)", testPerfIncrement<int>(1,false));
    testing::printSpeedResult("increment (1 thread, thread-view)", testPerfIncrement<int>(1,true));
    testing::printSpeedResult("value     (1 thread)", testPerfValue<int>(1));
    testing::printSpeedResult("increment (2 threads)", testPerfIncrement<int>(2,false));
    testing::printSpeedResult("increment (2 threads, thread-view)", testPerfIncrement<int>(2,true));
    testing::printSpeedResult("value     (2 thread)", testPerfValue<int>(2));
    testing::printSpeedResult("increment (5 threads)", testPerfIncrement<int>(5,false));
    testing::printSpeedResult("increment (5 threads, thread-view)", testPerfIncrement<int>(5,true));
    testing::printSpeedResult("value     (5 thread)", testPerfValue<int>(5));
    testing::printSpeedResult("increment (10 threads)", testPerfIncrement<int>(10,false));
    testing::printSpeedResult("increment (10 threads, thread-view)", testPerfIncrement<int>(10,true));
    testing::printSpeedResult("value     (10 thread)", testPerfValue<int>(10));
    testing::printSpeedResult("increment (50 threads)", testPerfIncrement<int>(50,false));
    testing::printSpeedResult("increment (50 threads, thread-view)", testPerfIncrement<int>(50,true));
    testing::printSpeedResult("value     (50 thread)", testPerfValue<int>(50));
}

void* increment(void* arg0) {
    static_cast<AtomicCounter<int>*>(arg0)->increment();
    return 0;
}

template <typename T>
double testPerfIncrement(unsigned threads, bool useThreadView)
{
    const std::size_t iters(10000);
    StopWatch stopWatch;
    double total(0);

    AtomicCounter<T> counter;
    pthread_t* pthreads = new pthread_t[threads];
    for(unsigned ii = 0; ii != threads; ++ii) {
        pthread_create(&(pthreads[ii]), 0, &increment, &counter);
    }
    for(unsigned ii = 0; ii != threads; ++ii) {
        pthread_join(pthreads[ii],0);
    }
    delete [] pthreads;

    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        typename AtomicCounter<T>::ThreadView view(counter.threadView());
        stopWatch.start();
        if( useThreadView ) { 
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                view.increment();
            }
        } else {
            for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
                counter.increment();
            }
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

template <typename T>
double testPerfValue(unsigned threads)
{
    const std::size_t iters(30000);
    StopWatch stopWatch;
    double total(0);
    
    AtomicCounter<T> counter;
    for(unsigned ii = 0; ii != threads; ++ii) {
        pthread_t pthread;
        pthread_create(&pthread, 0, &increment, &counter);
        pthread_join(pthread, 0);
    }

    T foo(0);
    for(std::size_t iter = 0; iter != iters; ++iter)
    {
        stopWatch.start();
        for(std::size_t count = 0; count != testing::STANDARD_SPEED_COUNT; ++count) {
            foo += counter.value();
        }
        stopWatch.stop();
        total += getTimevalAsMicros(stopWatch.elapsed());
        stopWatch.reset();
    }
    return testing::calcNanosPer(total, iters);
}

