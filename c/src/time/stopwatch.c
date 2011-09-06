
#include <assert.h>

#include "stopwatch.h"

#define PRO_STOPWATCH_IS_STARTED(sw) ((sw)->last_start.tv_sec != 0)

void pro_stopwatch_init(pro_stopwatch_t *const restrict stopwatch)
{
    pro_stopwatch_reset(stopwatch);
}

void pro_stopwatch_start(pro_stopwatch_t *const restrict stopwatch)
{
  assert(!PRO_STOPWATCH_IS_STARTED(stopwatch));
  gettimeofday(&(stopwatch->last_start), 0);
}

void pro_stopwatch_stop(pro_stopwatch_t *const restrict stopwatch)
{
  assert(PRO_STOPWATCH_IS_STARTED(stopwatch));
  struct timeval now, diff;
  gettimeofday(&now, 0);
  pro_timeval_diff(&diff, &now, &(stopwatch->last_start));
  pro_timeval_total(&(stopwatch->elapsed), &(stopwatch->elapsed), &diff);
  stopwatch->last_start.tv_sec = 0;
}

