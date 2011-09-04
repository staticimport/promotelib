#ifndef PROMOTE_TASK_HPP_
#define PROMOTE_TASK_HPP_

#include "ArrayDeque.hpp"

namespace promote
{
    class Task
    {
    public:
        struct TaskCompletedListener
        {
            virtual ~TaskCompletedListener() { }
            virtual void onTaskCompleted(Task* task) = 0;
        };

        Task(std::size_t const expectedListenerCount = 1);
        virtual ~Task() { }

        /* Const */
        bool completed() const  { return _completed; }

        /* Non-Const */
        void addListener(TaskCompletedListener* listener)
        {
            _listeners.pushBack(listener);
        }
        virtual void process() = 0;
        void setCompleted()
        { 
            _completed = true; 
            ArrayDeque<TaskCompletedListener*>::ConstIterator iter(_listeners);
            while( iter.hasNext() )
            {
              iter.next()->onTaskCompleted(this);
            }
        }
    private:
        ArrayDeque<TaskCompletedListener*> _listeners;
        bool _completed;
    };
}

#endif /* PROMOTE_TASK_HPP_ */

