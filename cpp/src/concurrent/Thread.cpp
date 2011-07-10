
#include "Thread.hpp"

using namespace promote;

Atomic<Thread::Id> Thread::NEXT_ID(0);

namespace promote {
    class PromoteThreadHelper
    {
    public:
        static void run(Thread* const thread) { thread->run(); }
    };
}

void* run_thread(void* arg0)
{
    Thread* const thread(static_cast<Thread*>(arg0));
    PromoteThreadHelper::run(thread);
    return 0;
}

Thread::Thread()
:   _id(NEXT_ID.fetchAndAdd(1)),
    _state(THREAD_STOPPED)
{ }

Thread::~Thread()
{
    if( _state.fetch() != THREAD_STOPPED ) {
        pthread_cancel(_pthread);
    }
}

void
Thread::join()
{
    if( _state.compareAndSwap(THREAD_RUNNING, THREAD_STOPPING) ) {
        pthread_join(_pthread, 0);
        _state.set(THREAD_STOPPED);
    } // TODO: else throw exception
}

void
Thread::start()
{
    if( _state.compareAndSwap(THREAD_STOPPED, THREAD_RUNNING) ) {
        pthread_create(&_pthread, 0, &run_thread, this);
    } // TODO: else throw exception
}

