#ifndef PROMOTE_MUTEX_HPP_
#define PROMOTE_MUTEX_HPP_

#include <pthread.h>

#include "Exception.hpp"

namespace promote
{
    class Mutex
    {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        Mutex();
        ~Mutex();

        void lock() 
        {
            if( pthread_mutex_lock(&_mutex) ) {
                throw Exception("Mutex Lock failed!");
            }
        }
        bool trylock()
        {
            return pthread_mutex_trylock(&_mutex) == 0;
        }
        void unlock()
        {
            if( pthread_mutex_unlock(&_mutex) ) {
                throw Exception("Mutex Unlock failed!");
            }
        }
    private:
        pthread_mutex_t _mutex;
    };
}

#endif /* PROMOTE_MUTEX_HPP_ */

