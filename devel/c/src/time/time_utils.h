#ifndef PROMOTE_TIME_UTILS_H_
#define PROMOTE_TIME_UTILS_H_

#include <iso646.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

/* Returns current time in milliseconds since the Epoch */
static inline uint64_t pmt_get_current_millis()
{
    struct timeval time;
    gettimeofday(&time, 0);
    return (time.tv_sec * ((uint64_t)1000)) + (time.tv_usec / 1000);
}

/* Converts timeval to microseconds */
static inline uint64_t pmt_timeval_to_micros(const struct timeval *restrict time)
{
    return (time->tv_sec * ((uint64_t)1000000)) + time->tv_usec;
}

/* Converts timeval to nanoseconds */
static inline uint64_t pmt_timeval_to_nanos(const struct timeval *restrict time)
{
    return (time->tv_sec * ((uint64_t)1000000000)) + (time->tv_usec * ((uint64_t)1000));
}

/** 
 * Subtract timevals and store in specified timeval.
 * Does NOT detect for underflow as it does NOT test that
 * first < second.
 **/
static inline void pmt_timeval_diff(struct timeval *restrict result,
                             const struct timeval* restrict first,
                             const struct timeval* restrict second)
{
    if( first->tv_usec >= second->tv_usec ) {
        result->tv_sec = first->tv_sec - second->tv_sec;
        result->tv_usec = first->tv_usec - second->tv_usec;
    } else {
        result->tv_sec = first->tv_sec - second->tv_sec - 1;
        result->tv_usec = 1000000 + first->tv_usec - second->tv_usec;
    }
}

/** 
 * Subtract timevals and store in specified timeval.
 * Does detect for underflow by checking if first < second.
 **/
static inline bool pmt_timeval_diff_safe(struct timeval *restrict result,
                                  const struct timeval* restrict first,
                                  const struct timeval* restrict second)
{
    if( first->tv_sec > second->tv_sec ) {
        pmt_timeval_diff(result, first, second);
        return true;
    } else if( first->tv_sec == second->tv_sec and first->tv_usec > second->tv_usec ) {
        result->tv_sec = 0;
        result->tv_usec = first->tv_usec - second->tv_usec;
        return true;
    } else {
        result->tv_sec = 0;
        result->tv_usec = 0;
        return false;
    }
}

/**
 * Add timevals an store in specified timeval.
 * Does NOT detect for overflow.
 **/
static inline void pmt_timeval_total(struct timeval *result,
                              const struct timeval* restrict first,
                              const struct timeval* restrict second)
{
    result->tv_sec = first->tv_sec + second->tv_sec;
    result->tv_usec = first->tv_usec + second->tv_usec;
    if( result->tv_usec >= 1000000 ) {
        result->tv_usec -= 1000000;
        ++(result->tv_sec);
    }
}

#endif /* PROMOTE_TIME_UTILS_H_ */

