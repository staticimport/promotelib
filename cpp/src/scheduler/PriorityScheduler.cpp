
#include "PriorityScheduler.hpp"

using namespace promote;

PriorityTask::PriorityTask(PriorityTask::priority_t const priority,
                           std::size_t const expectedListenerCount)
:   Task(expectedListenerCount),
    _priority(priority)
{
}

