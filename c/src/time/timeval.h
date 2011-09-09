#ifndef PROMOTE_timevalVAL_H
#define PROMOTE_timevalVAL_H

#include <stdint.h>

typedef struct pro_timeval {
  int32_t seconds;
  int32_t nanoseconds;
} pro_timeval_t;


/**
 * Prototypes
 **/

// Create tim
void pro_timeval_now_monotonic(pro_timeval_t* const restrict ts);

void pro_timeval_now_wall(pro_timeval_t* const restrict ts);


// Convert tim
static inline uint64_t
pro_timeval_as_micros(pro_timeval_t const* const restrict ts);

static inline uint64_t
pro_timeval_as_nanos(pro_timeval_t const* const restrict ts);

static inline void
pro_timeval_diff(pro_timeval_t* const result,
                   pro_timeval_t const* const first,
                   pro_timeval_t const* const second);

static inline void
pro_timeval_sum(pro_timeval_t* const result,
                pro_timeval_t const* const first,
                pro_timeval_t const* const second);

/**
 * Inline implemenation
 **/
static inline uint64_t
pro_timeval_as_micros(pro_timeval_t const* const restrict ts)
{
  return (ts->seconds * 1000000) + (ts->nanoseconds / 1000);
}

static inline uint64_t
pro_timeval_as_nanos(pro_timeval_t const* const restrict ts)
{
  return (((uint64_t)ts->seconds) * 1000000000) + ts->nanoseconds;
}

static inline void
pro_timeval_diff(pro_timeval_t* const result,
                 pro_timeval_t const* const first,
                 pro_timeval_t const* const second)
{
  result->seconds = first->seconds - second->seconds;
  result->nanoseconds = first->nanoseconds - second->nanoseconds;
  if (result->nanoseconds < 0) {
    --(result->seconds);
    result->nanoseconds += 1000000000;
  }
}

static inline void
pro_timeval_sum(pro_timeval_t* const result,
                pro_timeval_t const* const first,
                pro_timeval_t const* const second)
{
  result->seconds = first->seconds + second->seconds;
  result->nanoseconds = first->nanoseconds + second->nanoseconds;
  while (result->nanoseconds > 1000000000) {
    ++(result->seconds);
    result->nanoseconds -= 1000000000;
  }
}


#endif /* PROMOTE_TIMEVAL_H */

