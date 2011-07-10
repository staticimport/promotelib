#ifndef PROMOTE_TIER_SCHEDULER_HPP_
#define PROMOTE_TIER_SCHEDULER_HPP_

#include <stdint.h>

#include "ArrayList.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"

namespace promote
{
    class Allocator;

    class TierTask : public Task
    {
    public:
        typedef uint8_t tier_t;

        TierTask(tier_t const tier,
                 std::size_t const expectedListenerCount=1);
        virtual ~TierTask() { }

        /* Const */
        tier_t tier() const { return _tier; }
    protected:
        tier_t _tier;
    };

    template <typename TASK=TierTask>
    class TierScheduler : public Scheduler<TASK>
    {
    public:

        TierScheduler(Allocator& allocator);
        virtual ~TierScheduler();

        void run(std::size_t const maxTasks,
                 bool const repeats=true);
    private:
        void insertTask(TASK* const task);

        ArrayList<Queue<TASK*,false,false>*> _tiers;
        std::size_t _numTasks;
        std::size_t _counter;
    };
}

/***
 * Implementation
 ***/
template <typename TASK>
promote::TierScheduler<TASK>::TierScheduler(promote::Allocator& allocator)
:   Scheduler<TASK>(allocator),
    _tiers(10),
    _numTasks(0),
    _counter(0)
{
}

template <typename TASK>
promote::TierScheduler<TASK>::~TierScheduler()
{
    TASK* task(0);
    for(std::size_t ii = 0; ii != _tiers.size(); ++ii)
    {
        if( _tiers[ii] ) {
            while( _tiers[ii]->read(task) ) {
                Scheduler<TASK>::releaseTask(task);
            }
            delete _tiers[ii];
        }
    }
}

template <typename TASK>
void
promote::TierScheduler<TASK>::run(std::size_t const maxTasks,
                                  bool const repeats)
{
    TASK* task(0);

    // Flush _addedTasks queue
    while( (task = Scheduler<TASK>::getAddedTask()) ) {
        insertTask(task);
    }

    if( _numTasks == 0 ) return;
    std::size_t const tierCount(_tiers.size());
    for(std::size_t processed = 0; processed < maxTasks; ) {
        std::size_t const count(++_counter);
        std::size_t tier(0);
        do {
            // Process tasks in current tier
            Queue<TASK*,false,false>* const tierTasks(_tiers[tier]);
            if( tierTasks ) {
                std::size_t const tierTaskCount(tierTasks->size());
                for(std::size_t jj = 0; jj != tierTaskCount; ++jj) {
                    tierTasks->read(task);
                    task->process();
                    ++processed;
                    if( task->completed() ) {
                        Scheduler<TASK>::releaseTask(task);
                        --_numTasks;
                        if( _numTasks == 0 ) {
                            return;
                        }
                    } else if( task->tier() == tier ) { // tier hasn't changed
                        tierTasks->write(task);
                    } else {
                        insertTask(task);
                    }
                }
            }
            ++tier;
        } while( (count & ((1 << tier) - 1)) == 0 and (tier < tierCount) );
    }
}

template <typename TASK>
void 
promote::TierScheduler<TASK>::insertTask(TASK* const task)
{
    std::size_t const tier(static_cast<std::size_t>(task->tier()));
    for(std::size_t ii = _tiers.size(); ii <= tier; ++ii) {
        _tiers.pushBack(0);
    }
    Queue<TASK*,false,false>* tierTasks(_tiers[tier]);
    if( tierTasks == 0 ) {
        tierTasks = new Queue<TASK*,false,false>(4);
        _tiers[tier] = tierTasks;
    }
    tierTasks->write(task);
    ++_numTasks;
}

#endif /* PROMOTE_TIER_SCHEDULER_HPP_ */

