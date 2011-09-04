
#include "stopwatch.h"

int pmt_stopwatch_init(pmt_stopwatch_t *restrict stopwatch)
{
    pmt_stopwatch_reset(stopwatch);
    return 0;
}

