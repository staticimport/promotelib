
#include "time_utils.h"

void pro_timeval_diff(struct timeval* const result,
                      struct timeval const* const first,
                      struct timeval const* const second)
{
  result->tv_sec = first->tv_sec - second->tv_sec;
  if (first->tv_usec >= second->tv_usec) {
    result->tv_usec = first->tv_usec - second->tv_usec;
  } else {
    --(result->tv_sec);
    result->tv_usec = 1000000 + first->tv_usec - second->tv_usec;
  }
}

bool pro_timeval_diff_safe(struct timeval* const result,
                           struct timeval const* const first,
                           struct timeval const* const second)
{
  if (first->tv_sec > second->tv_sec or (first->tv_sec == second->tv_sec and
      first->tv_usec >= second->tv_usec))
  {
    pro_timeval_diff(result, first, second);
    return true;
  } else {
    result->tv_sec = 0;
    result->tv_usec = 0;
    return false;
  }
}

void pro_timeval_total(struct timeval *const result,
                       struct timeval const* const first,
                       struct timeval const* const second)
{
  result->tv_sec = first->tv_sec + second->tv_sec;
  result->tv_usec = first->tv_usec + second->tv_usec;
  if (result->tv_usec >= 1000000) {
    result->tv_usec -= 1000000;
    ++(result->tv_sec);
  }
}

