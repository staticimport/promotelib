#ifndef PROMOTE_TIME_UTILS_H_
#define PROMOTE_TIME_UTILS_H_

#include <iso646.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

/* Returns current time in milliseconds since the Epoch */
static inline uint64_t pro_get_current_millis();

/* Converts timeval to microseconds */
static inline uint64_t 
pro_timeval_to_micros(struct timeval const* const restrict time);

/* Converts timeval to nanoseconds */
static inline uint64_t 
pro_timeval_to_nanos(const struct timeval const* const restrict time);

/** 
 * Subtract timevals and store in specified timeval.
 * Does NOT detect for underflow as it does NOT test that
 * first < second.
 **/
void pro_timeval_diff(struct timeval *const result,
                      struct timeval const *const first,
                      struct timeval const *const second);

/** 
 * Subtract timevals and store in specified timeval.
 * Does detect for underflow by checking if first < second.
 **/
bool pro_timeval_diff_safe(struct timeval *const result,
                           struct timeval const *const first,
                           struct timeval const *const second);

/**
 * Add timevals an store in specified timeval.
 * Does NOT detect for overflow.
 **/
void pro_timeval_total(struct timeval* const result,
                       struct timeval const* const first,
                       struct timeval const* const second);


/**
 * Inline Implementations
 **/
static inline uint64_t pro_get_current_millis()
{
    struct timeval time;
    gettimeofday(&time, 0);
    return (time.tv_sec * ((uint64_t)1000)) + (time.tv_usec / 1000);
}

/* Converts timeval to microseconds */
static inline uint64_t 
pro_timeval_to_micros(struct timeval const *const time)
{
    return (time->tv_sec * ((uint64_t)1000000)) + time->tv_usec;
}

/* Converts timeval to nanoseconds */
static inline uint64_t 
pro_timeval_to_nanos(struct timeval const* const time)
{
    return (time->tv_sec * ((uint64_t)1000000000)) + 
      (time->tv_usec * ((uint64_t)1000));
}

#endif /* PROMOTE_TIME_UTILS_H_ */

