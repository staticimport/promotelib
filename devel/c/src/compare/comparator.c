
#include <assert.h>

#include "comparator.h"

void pmt_comparator_init(pmt_comparator_t* const restrict comp,
                         pmt_order_t (*compare)(void*,pmt_universal_t,pmt_universal_t),
                         void* arg0)
{
    assert(comp);
    comp->compare = compare;
    comp->arg0 = arg0;
}

void pmt_comparator_init_copy(pmt_comparator_t* const restrict dest,
                              pmt_comparator_t* const restrict source)
{
    assert(dest);
    assert(source);
    dest->compare = source->compare;
    dest->arg0 = source->arg0;
}

