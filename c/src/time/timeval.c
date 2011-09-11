
#ifndef __APPLE__
#include <time.h>
#else
#include <sys/time.h>
#endif

#include "timeval.h"

void pro_timeval_now_monotonic(pro_timeval_t* const restrict ts)
{
#ifndef __APPLE__
  struct timespec tspec;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tspec);
  ts->seconds = (int32_t)tspec.tv_sec;
  ts->nanoseconds = (int32_t)tspec.tv_nsec;
#else
  pro_timeval_now_wall(ts); // NOT GOOD
#endif
}

void pro_timeval_now_wall(pro_timeval_t* const restrict ts)
{
#ifndef __APPLE__
  struct timespec tspec;
  clock_gettime(CLOCK_REALTIME, &tspec);
  ts->seconds = (int32_t)tspec.tv_sec;
  ts->nanoseconds = (int32_t)tspec.tv_nsec;
#else
  struct timeval tv;
  gettimeofday(&tv, 0);
  ts->seconds = (int32_t)tv.tv_sec;
  ts->nanoseconds = (int32_t)tv.tv_usec * 1000;
#endif
}

