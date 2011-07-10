#ifndef PROMOTE_THREAD_HPP_
#define PROMOTE_THREAD_HPP_

#include <pthread.h>

#include "Atomic.hpp"
#include "Hash.hpp"
#include "Utilities.hpp"

namespace promote
{
    class Thread
    {
    public:
        typedef unsigned Id;
        
        enum State { THREAD_RUNNING, THREAD_STOPPING, THREAD_STOPPED };

        Thread();
        virtual ~Thread();
        
        // Const
        hash_t hashcode() const                                     { return static_cast<hash_t>(_id); }
        Id id() const                                               { return _id; }
        Passable<pthread_t const>::Type pthread() const             { return _pthread; }
        State state() const                                         { return _state.fetch(); }
        bool operator==(Thread const& thread) const { return id() == thread.id(); }
        bool operator!=(Thread const& thread) const { return id() != thread.id(); }
        bool operator<(Thread const& thread) const  { return id() < thread.id(); }
        bool operator<=(Thread const& thread) const { return id() <= thread.id(); }
        bool operator>(Thread const& thread) const  { return id() > thread.id(); }
        bool operator>=(Thread const& thread) const { return id() >= thread.id(); }

        // Non-Const
        void join();
        Passable<pthread_t>::Type pthread()                { return _pthread; }
        void start();
    protected:
        virtual void run() = 0;
    private:
        friend class PromoteThreadHelper;

        static Atomic<Id> NEXT_ID;

        Id const _id;
        pthread_t _pthread;
        Atomic<State> _state;
    };

    template <typename T>
    class ReturnValueThread : public Thread
    {
    public:
        ReturnValueThread() { }
        virtual ~ReturnValueThread() { }

        // Const
        typename Passable<T const>::Type returnValue() const    { return _result; } // TODO: throw exception

        // Non-Const
        typename Passable<T>::Type returnValue()                { return _result; } // TODO: throw exception
    protected:
        void run() { run(_result); }
        virtual void run(T& result);
    private: 
        T _result;
    };
}

#endif /* PROMOTE_THREAD_HPP_ */

