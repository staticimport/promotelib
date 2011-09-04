#ifndef PROMOTE_ATOMIC_COUNTER_HPP_
#define PROMOTE_ATOMIC_COUNTER_HPP_

#include <pthread.h>
#include <cstring>

#include "Mutex.hpp"
#include "Threads.hpp"

namespace promote
{
    template <typename T=int>
    class AtomicCounter {
    public:

        AtomicCounter() : _headCounter(0) { }
        ~AtomicCounter();

        // Const
        T value() const {
            T total(0);
            PerThreadCounter* ptc(_headCounter);
            while( ptc ) {
                total += ptc->counter;
                ptc = ptc->next;
            }
            return total;
        }

        // Non-Const
        void decrement() { findMyPerThreadCounter().counter--; }
        void increment() { findMyPerThreadCounter().counter++; }
    private:
        struct PerThreadCounter {
            PerThreadCounter(PerThreadCounter* const next_, pthread_t const thread_)
            :   next(next_), thread(thread_), counter(0)
            { }

            PerThreadCounter* const next;
            pthread_t const thread;
            T volatile counter;
        };
    public:
        class ThreadView {
        public:
            ThreadView(PerThreadCounter& counter) : _counter(counter) { }
            ThreadView(ThreadView const& view) : _counter(view._counter) { }

            void decrement() { _counter.counter--; }
            void increment() { _counter.counter++; }
        private:
            PerThreadCounter& _counter;
        };
        
        ThreadView threadView() {
            return ThreadView(findMyPerThreadCounter());
        }

    private:
        PerThreadCounter* findThreadCounter(pthread_t const thread,
                                            PerThreadCounter* const begin,
                                            PerThreadCounter* const end)
        {
            PerThreadCounter* counter(begin);
            while( counter != end ) {
                if( Threads::equal(thread, counter->thread) ) {
                    return counter;
                } else {
                    counter = counter->next;
                }
            }
            return NULL;
        }

        PerThreadCounter& findMyPerThreadCounter() {
            pthread_t const thread(pthread_self());
            PerThreadCounter* const unlockedHeadCounter(_headCounter);
            PerThreadCounter* counter(findThreadCounter(thread, unlockedHeadCounter, NULL));
            if( counter == NULL ) {
                _newThreadMutex.lock();
                PerThreadCounter* const lockedHeadCounter(_headCounter);
                counter = findThreadCounter(thread, lockedHeadCounter, unlockedHeadCounter);
                if( counter == NULL ) {
                    _headCounter = (counter = new PerThreadCounter(lockedHeadCounter, thread));
                }
                _newThreadMutex.unlock();
            }
            return *counter;
        }

        PerThreadCounter* volatile _headCounter;
        Mutex _newThreadMutex;
    };
}

/***
 * Implementation
 ***/
template <typename T>
promote::AtomicCounter<T>::~AtomicCounter()
{
    PerThreadCounter* counter(_headCounter);
    while(counter) {
        PerThreadCounter* const temp(counter->next);
        delete counter;
        counter = temp;
    }
}

#endif /* PROMOTE_ATOMIC_COUNTER_HPP_ */

