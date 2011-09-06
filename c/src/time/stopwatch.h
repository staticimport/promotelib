#ifndef PROMOTE_STOPWATCH_H_
#define PROMOTE_STOPWATCH_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

#include "time_utils.h"

typedef struct pro_stopwatch {
    struct timeval elapsed;
    struct timeval last_start;
} pro_stopwatch_t;

/**
 * Prototypes
 **/
void pro_stopwatch_init(pro_stopwatch_t *const restrict stopwatch);

static inline void 
pro_stopwatch_elapsed(pro_stopwatch_t const *const restrict stopwatch,
                      struct timeval *const restrict result);

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
                      struct timeval *const restrict result)
{
    result->tv_sec = stopwatch->elapsed.tv_sec;
    result->tv_usec = stopwatch->elapsed.tv_usec;
}

static inline uint64_t 
pro_stopwatch_elapsed_micros(pro_stopwatch_t const* const restrict stopwatch)
{
    return ((uint64_t)(stopwatch->elapsed.tv_sec) * 1000000) +
           (uint64_t)(stopwatch->elapsed.tv_usec);
}

static inline uint64_t 
pro_stopwatch_elapsed_nanos(pro_stopwatch_t const* const restrict stopwatch)
{
    return ((uint64_t)(stopwatch->elapsed.tv_sec) * 1000000000) +
           ((uint64_t)(stopwatch->elapsed.tv_usec) * 1000);
}

static inline void 
pro_stopwatch_reset(pro_stopwatch_t *restrict stopwatch)
{
  stopwatch->last_start.tv_sec = 0;
  stopwatch->elapsed.tv_sec = 0;
  stopwatch->elapsed.tv_usec = 0;
}
#endif /* PROMOTE_STOPWATCH_H_ */

