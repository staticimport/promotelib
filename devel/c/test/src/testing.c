#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testing.h"

void pmt_print_perf_result(char const *const name,
                           uint64_t const iters,
                           uint64_t const total_nanos)
{
    char buf[128];
    char name_with_iters_buf[128];
    char nanos_buf[128];

    // Name
    unsigned const name_with_iters_len = 
      sprintf(name_with_iters_buf, "%s x %llu", name,
              (unsigned long long)iters);

    // Time
    double const ave_nanos = ((double)total_nanos) / iters;
    const unsigned nanos_len = 
      sprintf(nanos_buf, "total: %llu, avg: %.3f", 
              (unsigned long long)total_nanos, ave_nanos);

    memcpy(buf, name_with_iters_buf, name_with_iters_len);;
    memset(buf + name_with_iters_len, '.', 
           80 - name_with_iters_len - nanos_len);
    memcpy(buf + 80 - nanos_len, nanos_buf, nanos_len);
    printf("%.*s ns\n", 80, buf);
}

void pmt_print_unit_result(const char* name,
                           const char* error)
{
    char buf[128];
    const bool is_error = (error != 0) && (strlen(error) != 0);
    const char* const result = is_error ? "fail" : "pass";
    const unsigned name_len = strlen(name);
    const unsigned result_len = strlen(result);
    const unsigned fill_len = 70 - name_len - result_len;

    memset(buf, '.', fill_len);
    if( is_error ) { 
        printf("%s%.*s\033[1m%s\033[0m\n\033[7mERROR: %s\033[0m\n", 
               name, fill_len, buf, result, error);
    } else {
        printf("%s%.*s%s\n", name, fill_len, buf, result);
    }
}

