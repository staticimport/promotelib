#ifndef PROMOTE_FIFO_SCHEDULER_HPP_
#define PROMOTE_FIFO_SCHEDULER_HPP_

#include "Queue.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"

namespace promote
{
    class Allocator;

    template <typename TASK=Task>
    class FIFOScheduler : public Scheduler<TASK>
    {
    public:
        FIFOScheduler(Allocator& allocator,
                      std::size_t const expectedTaskCount=16);
        virtual ~FIFOScheduler();

        void run(std::size_t const maxTasks,
                 bool const repeats=true);
    private:
        Queue<TASK*,false,false> _taskQueue;
    };
}

/***
 * Implementation
 ***/
template <typename TASK>
promote::FIFOScheduler<TASK>::FIFOScheduler(promote::Allocator& allocator,
                                            std::size_t const expectedTaskCount)
:   Scheduler<TASK>(allocator),
    _taskQueue(expectedTaskCount)
{
}

template <typename TASK>
promote::FIFOScheduler<TASK>::~FIFOScheduler()
{
    TASK* task(0);
    while( _taskQueue.read(task) ) {
        Scheduler<TASK>::releaseTask(task);
    }
}

template <typename TASK>
void
promote::FIFOScheduler::run(std::size_t const maxTasks,
                            bool const repeats)
{
    TASK* task(0);

    // Flush _addedTasks queue
    while( (task = getAddedTask()) ) {
        _taskQueue.write(task);
    }

    std::size_t const taskCount(_taskQueue.size());
    if( taskCount ) {
        for(std::size_t ii = 0; ii != maxTasks; ++ii) {
            _taskQueue.read(task);
            task->process();
            if( task->completed() ) {
                releaseTask(task);
                if( _taskQueue.isEmpty() ) {
                    return;
                }
            } else {
                _taskQueue.write(task);
                if( !repeats ) {
                    return;
                }
            }
        }
    }
}

#endif /* PROMOTE_FIFO_SCHEDULER_HPP_ */

