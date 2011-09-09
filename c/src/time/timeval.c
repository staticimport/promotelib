
#include <time.h>

#include "timeval.h"

void pro_timeval_now_monotonic(pro_timeval_t* const restrict ts)
{
  struct timespec tspec;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tspec);
  ts->seconds = (int32_t)tspec.tv_sec;
  ts->nanoseconds = (int32_t)tspec.tv_nsec;
}

void pro_timeval_now_wall(pro_timeval_t* const restrict ts)
{
  struct timespec tspec;
  clock_gettime(CLOCK_REALTIME, &tspec);
  ts->seconds = (int32_t)tspec.tv_sec;
  ts->nanoseconds = (int32_t)tspec.tv_nsec;
}

