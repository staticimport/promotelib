#ifndef PROMOTE_SCHEDULER_HPP_
#define PROMOTE_SCHEDULER_HPP_

#include "Allocator.hpp"
#include "Queue.hpp"
#include "Task.hpp"

namespace promote
{
    template <typename TASK=Task>
    class Scheduler : public Task
    {
    public:
        Scheduler(Allocator& allocator);
        virtual ~Scheduler();

        void addTask(TASK* const task)
        {
            _addedTasks.write(task);
        }

        virtual void process() { run(100000, false); }
        virtual void run(std::size_t const maxTasks,
                         bool const repeats=true) = 0;
    protected:
        TASK* getAddedTask()
        {
            TASK* task(0);
            _addedTasks.read(task);
            return task;
        }

        void releaseTask(TASK* const task)
        {
            task->~TASK();
            _allocator.release(task);
        }
    private:
        Allocator& _allocator;
        Queue<TASK*,false,true> _addedTasks;
    };
}

/***
 * Implementation
 ***/
template <typename TASK>
promote::Scheduler<TASK>::Scheduler(promote::Allocator& allocator)
:   _allocator(allocator),
    _addedTasks(2)
{
}

template <typename TASK>
promote::Scheduler<TASK>::~Scheduler()
{
    TASK* task(0);
    while( (task = getAddedTask()) ) {
        releaseTask(task);
    }
}

#endif /* PROMOTE_SCHEDULER_HPP_ */

