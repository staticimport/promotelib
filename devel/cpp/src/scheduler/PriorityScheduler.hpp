#ifndef PROMOTE_PRIORITY_SCHEDULER_HPP_
#define PROMOTE_PRIORITY_SCHEDULER_HPP_

#include <cstddef>

#include "BinaryHeap.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"

namespace promote
{
    template <typename TASK> class PriorityScheduler;

    class PriorityTask : public Task
    {
    public:
        typedef long priority_t;

        PriorityTask(priority_t const priority,
                     std::size_t const expectedListenerCount = 1);
        virtual ~PriorityTask() { }

        priority_t priority() const     { return _priority; }
        virtual void process() = 0;
    protected:
        priority_t _priority;
    };

    template <typename TASK>
    class PriorityScheduler : public Scheduler<TASK>
    {
    private:
        class Entry
        {
        public:
            Entry() : _task(0) { }
            Entry(TASK* const task) : _task(task) { }

            /* Const */
            PriorityTask::priority_t priority() const { return _priority; }
            TASK* task() const { return _task; }
            bool operator==(Entry const& entry) const
            {
                return _priority == entry.priority();
            }
            bool operator!=(Entry const& entry) const
            {
                return _priority != entry.priority();
            }
            bool operator<(Entry const& entry) const
            {
                return _priority < entry.priority();
            }
            bool operator<=(Entry const& entry) const
            {
                return _priority <= entry.priority();
            }
            bool operator>(Entry const& entry) const
            {
                return _priority > entry.priority();
            }
            bool operator>=(Entry const& entry) const
            {
                return _priority >= entry.priority();
            }

            /* Non-Const */
            PriorityTask::priority_t& priority() { return _priority; }
            Entry& operator=(Entry const& entry)
            {
                _task = entry.task();
                _priority = _task->priority();
                return *this;
            }
        private:
            TASK* _task;
            PriorityTask::priority_t _priority;
        };
    public:
        PriorityScheduler(Allocator& allocator,
                          std::size_t const expectedTaskCount=16);
        virtual ~PriorityScheduler();

        void run(std::size_t const maxTasks,
                 bool const repeats=true);
    private:
        BinaryHeap<Entry,true,false> _tasks;
    };
}

/***
 * Implementation
 ***/
template <typename TASK>
promote::PriorityScheduler<TASK>::PriorityScheduler(promote::Allocator& allocator,
                                                    std::size_t const expectedTaskCount)
:   promote::Scheduler<TASK>(allocator),
    _tasks(expectedTaskCount)
{
}

template <typename TASK>
void
promote::PriorityScheduler<TASK>::run(std::size_t const maxTasks,
                                      bool const repeats)
{
    TASK* task(0);

    // Flush added tasks
    while( (task = Scheduler<TASK>::getAddedTask()) ) {
        _tasks.add(Entry(task));
    }

    if( _tasks.isEmpty() ) return;
    for(std::size_t ii = 0; ii != maxTasks; ++ii)
    {
        Entry& entry(_tasks.peek());
        TASK* const task(entry.task());
        task->process();
        if( task->completed() ) {
            releaseTask(task);
            _tasks.pop();
            if( _tasks.isEmpty() ) {
                return;
            }
        } else {
            PriorityTask::priority_t const oldPriority(entry.priority());
            entry.priority() = task->priority();
            if( oldPriority < entry.priority() ) {
                _tasks.pop();
                _tasks.push(Entry(task));
            }
        }
    }
}

#endif /* PROMOTE_PRIORITY_SCHEDULER_HPP_ */

