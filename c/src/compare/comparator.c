
#include <assert.h>

#include "comparator.h"

void pro_comparator_init(pro_comparator_t* const restrict comp,
                         pro_order_t (*compare)(void*,pro_item_t const,pro_item_t const),
                         void* arg0)
{
    assert(comp);
    comp->compare = compare;
    comp->arg0 = arg0;
}

void pro_comparator_init_copy(pro_comparator_t* const restrict dest,
                              pro_comparator_t* const restrict source)
{
    assert(dest);
    assert(source);
    dest->compare = source->compare;
    dest->arg0 = source->arg0;
}

