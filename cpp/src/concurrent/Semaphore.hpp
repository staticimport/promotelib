#ifndef PROMOTE_SEMAPHORE_HPP_
#define PROMOTE_SEMAPHORE_HPP_

extern "C" {
#ifndef __APPLE__
#include <errno.h>
#include <semaphore.h>
#else
#include <mach/semaphore.h>
#include <mach/task.h>
#endif
}

#include "Exception.hpp"


namespace promote
{
    class Semaphore
    {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        explicit Semaphore(int initValue=1);
        ~Semaphore();

        void signal()
        {
#ifndef __APPLE__
            if( sem_post(&_underlying) ) {
#else
            if( semaphore_signal(_machSemaphore) ) {
#endif
                throw Exception("Semaphore signal failed."); //TODO: real error
            }
        }

        bool tryWait()
        {
#ifndef __APPLE__
            if( sem_trywait(&_underlying) ) {
                if( errno == EAGAIN ) {
                    return false;
                } else {
                    throw Exception("Semaphore trywait failed."); // TODO: real error
                }
#else
            mach_timespec_t waitTime = { 0, 1 };
            if( semaphore_timedwait(_machSemaphore, waitTime) ) {
                return false;
#endif
            } else {
                return true;
            }
        }

#ifndef __APPLE__
        int value()
        {
            int val;
            if( sem_getvalue(&_underlying, &val) ) {
                throw Exception("Semaphores value failed."); // TODO: real error
            }
            return val;
        }
#endif

        void wait()
        {
#ifndef __APPLE__
            if( sem_wait(&_underlying) ) {
#else
            if( semaphore_wait(_machSemaphore) ) {
#endif
                throw Exception("Semaphores wait failed."); // TODO: real error
            }
        }
    private:
#ifndef __APPLE__
        sem_t _underlying;
#else
        semaphore_t _machSemaphore;
#endif
    };
}

#endif /* PROMOTE_SEMAPHORE_HPP_ */

