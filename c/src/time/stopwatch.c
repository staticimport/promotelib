
#include <assert.h>

#include "stopwatch.h"

#define PRO_STOPWATCH_IS_STARTED(sw) ((sw)->last_start.seconds != 0)

void pro_stopwatch_init(pro_stopwatch_t *const restrict stopwatch)
{
  pro_stopwatch_reset(stopwatch);
}

void pro_stopwatch_start(pro_stopwatch_t *const restrict stopwatch)
{
  assert(!PRO_STOPWATCH_IS_STARTED(stopwatch));
  pro_timeval_now_monotonic(&(stopwatch->last_start));
}

void pro_stopwatch_stop(pro_stopwatch_t *const restrict stopwatch)
{
  assert(PRO_STOPWATCH_IS_STARTED(stopwatch));
  pro_timeval_t now, diff;
  pro_timeval_now_monotonic(&now);
  pro_timeval_diff(&diff, &now, &(stopwatch->last_start));
  pro_timeval_sum(&(stopwatch->elapsed), &(stopwatch->elapsed), &diff);
  stopwatch->last_start.seconds = 0;
}

