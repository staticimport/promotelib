#ifndef PROMOTE_STOPWATCH_H_
#define PROMOTE_STOPWATCH_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

#include "timeval.h"

typedef struct pro_stopwatch {
  pro_timeval_t elapsed;
  pro_timeval_t last_start;
} pro_stopwatch_t;

/**
 * Prototypes
 **/
void pro_stopwatch_init(pro_stopwatch_t *const restrict stopwatch);

static inline void 
pro_stopwatch_elapsed(pro_stopwatch_t const *const restrict stopwatch,
                      pro_timeval_t *const restrict result);

static inline uint64_t 
pro_stopwatch_elapsed_micros(pro_stopwatch_t const* const restrict stopwatch);

static inline uint64_t 
pro_stopwatch_elapsed_nanos(pro_stopwatch_t const* const restrict stopwatch);

static inline void 
pro_stopwatch_reset(pro_stopwatch_t *const restrict stopwatch);

void pro_stopwatch_start(pro_stopwatch_t *const restrict stopwatch);

void pro_stopwatch_stop(pro_stopwatch_t *const restrict stopwatch);

/** 
 * Implementation
 **/
static inline void 
pro_stopwatch_elapsed(pro_stopwatch_t const *const restrict stopwatch,
                      pro_timeval_t *const restrict result)
{
  result->seconds= stopwatch->elapsed.seconds;
  result->nanoseconds = stopwatch->elapsed.nanoseconds;
}

static inline uint64_t 
pro_stopwatch_elapsed_micros(pro_stopwatch_t const* const restrict stopwatch)
{
  return pro_timeval_as_micros(&(stopwatch->elapsed));
}

static inline uint64_t 
pro_stopwatch_elapsed_nanos(pro_stopwatch_t const* const restrict stopwatch)
{
  return pro_timeval_as_nanos(&(stopwatch->elapsed));
}

static inline void 
pro_stopwatch_reset(pro_stopwatch_t *restrict stopwatch)
{
  stopwatch->last_start.seconds = 0;
  stopwatch->elapsed.seconds = 0;
  stopwatch->elapsed.nanoseconds = 0;
}

#endif /* PROMOTE_STOPWATCH_H_ */

