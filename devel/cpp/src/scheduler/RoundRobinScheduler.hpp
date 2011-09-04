#ifndef PROMOTE_ROUND_ROBIN_SCHEDULER_HPP_
#define PROMOTE_ROUND_ROBIN_SCHEDULER_HPP_

#include "Allocator.hpp"
#include "Queue.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"

namespace promote
{
    template <typename TASK=Task>
    class RoundRobinScheduler : public Scheduler<TASK>
    {
    public:
        RoundRobinScheduler(Allocator& allocator,
                            std::size_t const expectedTaskCount=16);
        virtual ~RoundRobinScheduler();

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
promote::RoundRobinScheduler<TASK>::RoundRobinScheduler(promote::Allocator& allocator,
                                                        std::size_t const expectedTaskCount)
:   Scheduler<TASK>(allocator),
    _taskQueue(expectedTaskCount)
{
}

template <typename TASK>
promote::RoundRobinScheduler<TASK>::~RoundRobinScheduler()
{
    TASK* task(0);
    while( _taskQueue.read(task) ) {
        Scheduler<TASK>::releaseTask(task);
    }
}

template <typename TASK>
void
promote::RoundRobinScheduler<TASK>::run(std::size_t const maxTasks,
                                        bool const repeats)
{
    TASK* task(0);

    // Flush _addedTasks queue
    while( (task = Scheduler<TASK>::getAddedTask()) ) {
        _taskQueue.write(task);
    }

    std::size_t const taskCount(_taskQueue.size());
    if( taskCount ) {
        std::size_t const numToProcess(repeats ? maxTasks : std::min<std::size_t>(maxTasks, taskCount));
        for(std::size_t ii = 0; ii != numToProcess; ++ii) {
            _taskQueue.read(task);
            task->process();
            if( task->completed() ) {
                releaseTask(task);
                if( _taskQueue.isEmpty() ) {
                    return;
                }
            } else {
                _taskQueue.write(task);
            }
        }
    }
}

#endif /* PROMOTE_ROUND_ROBIN_SCHEDULER_HPP_ */

