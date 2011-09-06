#ifndef PROMOTE_TESTING_H_
#define PROMOTE_TESTING_H_

#include <stdint.h>

#define PROMOTE_STANDARD_SPEED_COUNT (1000)
#define PROMOTE_STANDARD_SPEED_ITERS (10000)

static inline double pro_testing_calc_nanos_per(uint64_t const total_micros, 
                                                unsigned const count)
{
    return (total_micros * 1000.0) / count;
}

void pro_print_perf_result(char const* const name, double const nanos);

void pro_print_perf_iters_result(char const *const  name,
                                 uint64_t const iterations,
                                 double const total_nanos);

void pro_print_unit_result(const char* name,
                           const char* error);

#endif /* PROMOTE_TESTING_H_ */

