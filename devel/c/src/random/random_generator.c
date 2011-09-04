
#include <assert.h>

#include "random_generator.h"

bool
pmt_randgen_init(pmt_randgen_t *const restrict randgen,
                 pmt_rand_t (*gen)(void* arg0),
                 void* arg0)
{
    assert(randgen);

    randgen->gen = gen;
    randgen->arg0 = arg0;
    return true;
}

bool
pmt_randgen_init_copy(pmt_randgen_t *const restrict dest,
                      pmt_randgen_t *const restrict source)
{
    assert(dest);
    assert(source);

    dest->gen = source->gen;
    dest->arg0 = source->arg0;
    return true;
}

