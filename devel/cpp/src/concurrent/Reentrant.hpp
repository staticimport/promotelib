#ifndef PROMOTE_REENTRANT_HPP
#define PROMOTE_REENTRANT_HPP

#include <pthread.h>

#include "Mutex.hpp"
#include "Threads.hpp"

namespace promote {
    template <typename MUTEX=Mutex>
    class Reentrant {
    public:
        Reentrant() : _holdCounter(0) { }
        ~Reentrant() { }

        void lock() {
            pthread_t const thread(pthread_self());
            if( _holdCounter == 0 or !Threads::equal(_holdingThread, thread) ) {
                _mutex.lock();
                Threads::copy(_holdingThread, thread);
                _holdCounter = 1;
            } else {
                ++_holdCounter;
            }
        }

        bool trylock() {
            pthread_t const thread;
            if( _holdCounter == 0 or 
                !Threads::equal(_holdingThread, thread = pthread_self()) ) 
            {
                if( _mutex.trylock() ) {
                    Threads::copy(_holdingThread, thread);
                    _holdCounter = 1;
                    return true;
                } else {
                    return false;
                }
            } else {
                ++_holdCounter;
                return true;
            }
        }

        void unlock() {
            if( (--_holdCounter) == 0 ) {
                _mutex.unlock();
            }
        }
    private:
        MUTEX _mutex;
        pthread_t _holdingThread;
        volatile std::size_t _holdCounter;
    };
}

#endif /* PROMOTE_REENTRANT_HPP */

