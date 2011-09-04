#ifndef PROMOTE_READ_WRITE_MUTEX_HPP_
#define PROMOTE_READ_WRITE_MUTEX_HPP_

#include <pthread.h>

#include "Exception.hpp"

namespace promote
{
    class ReadWriteMutex {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        ReadWriteMutex();
        ~ReadWriteMutex();

        void readLock() {
            if( pthread_rwlock_rdlock(&_rwmutex) ) {
                throw Exception("ReadWriteMutex read lock failed!");
            }
        }
        bool tryReadLock() {
            return pthread_rwlock_tryrdlock(&_rwmutex) == 0;
        }
        void writeLock() {
            if( pthread_rwlock_wrlock(&_rwmutex) ) {
                throw Exception("ReadWriteMutex write lock failed!");
            }
        }
        bool tryWriteLock() {
            return pthread_rwlock_trywrlock(&_rwmutex) == 0;
        }
        void unlock() {
            if( pthread_rwlock_unlock(&_rwmutex) ) {
                throw Exception("ReadWriteMutex unlock failed!");
            }
        }
    private:
        pthread_rwlock_t _rwmutex;
    };
}

#endif /* PROMOTE_READ_WRITE_LOCK_HPP_ */

