#ifndef PROMOTE_STOP_WATCH_H_
#define PROMOTE_STOP_WATCH_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

#include "time_utils.h"

typedef struct pmt_stopwatch {
    struct timeval elapsed;
    struct timeval last_start;
    bool started;
} pmt_stopwatch_t;

int pmt_stopwatch_init(pmt_stopwatch_t *restrict stopwatch);

static inline void pmt_stopwatch_elapsed(const pmt_stopwatch_t *restrict stop_watch,
                                         struct timeval *restrict result)
{
    result->tv_sec = stop_watch->elapsed.tv_sec;
    result->tv_usec = stop_watch->elapsed.tv_usec;
}

static inline
uint64_t pmt_stopwatch_elapsed_micros(pmt_stopwatch_t const* const restrict stop_watch)
{
    return ((uint64_t)(stop_watch->elapsed.tv_sec) * 1000000) +
           (uint64_t)(stop_watch->elapsed.tv_usec);
}

static inline
uint64_t pmt_stopwatch_elapsed_nanos(pmt_stopwatch_t const* const restrict stop_watch)
{
    return ((uint64_t)(stop_watch->elapsed.tv_sec) * 1000000000) +
           ((uint64_t)(stop_watch->elapsed.tv_usec) * 1000);
}

static inline void pmt_stopwatch_reset(pmt_stopwatch_t *restrict stop_watch)
{
    stop_watch->elapsed.tv_sec = 0;
    stop_watch->elapsed.tv_usec = 0;
    stop_watch->started = false;
}

static inline bool pmt_stopwatch_start(pmt_stopwatch_t *restrict stop_watch)
{
    if( !stop_watch->started ) {
        gettimeofday(&(stop_watch->last_start), 0);
        return (stop_watch->started = true);
    } else {
        return false;
    }
}

static inline bool pmt_stopwatch_stop(pmt_stopwatch_t *restrict stop_watch)
{
    if( stop_watch->started ) {
        struct timeval now, diff;
        gettimeofday(&now, 0);
        pmt_timeval_diff(&diff, &now, &(stop_watch->last_start));
        pmt_timeval_total(&(stop_watch->elapsed),
                          &(stop_watch->elapsed),
                          &diff);
        stop_watch->started = false;
        return true;
    } else {
        return false;
    }
}

#endif /* PROMOTE_STOP_WATCH_H_ */

