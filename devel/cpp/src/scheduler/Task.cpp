
#include "Task.hpp"

using namespace promote;

Task::Task(std::size_t const expectedListenerCount)
:   _listeners(expectedListenerCount),
    _completed(false)
{
}

