#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testing.h"

void pmt_print_perf_result(char const *const name,
                           double const average_nanos)
{
    char buf[128];
    char nanos_buf[32];
    
    //double const ave_nanos = ((double)elapsed_nanos) / iter_count;
    const unsigned nanos_len = sprintf(nanos_buf, "%.3f", average_nanos);
    const unsigned name_len = strlen(name);

    memcpy(buf, name, name_len);
    memset(buf + name_len, '.', 70 - name_len - nanos_len);
    memcpy(buf + 70 - nanos_len, nanos_buf, nanos_len);
    printf("%.*s ns\n", 70, buf);
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

