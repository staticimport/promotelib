
#include <stdlib.h>

#include "random_functions.h"

pmt_rand_t pmt_random_standard(void* notUsed)
{
#ifdef x86_64
    return ((pmt_rand_t)random()) << 32 | (pmt_rand_t)random();
#else
    return (pmt_rand_t)random();
#endif
}

