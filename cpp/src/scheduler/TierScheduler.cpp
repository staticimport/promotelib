
#include "TierScheduler.hpp"

using namespace promote;

TierTask::TierTask(TierTask::tier_t const tier,
                   std::size_t const expectedListenerCount)
:   Task(expectedListenerCount),
    _tier(tier)
{
}

