#ifndef PROMOTE_COMPARATOR_H_
#define PROMOTE_COMPARATOR_H_

#include "universal.h"

typedef enum pmt_order {
    PMT_ORDER_INCREASING = -1,
    PMT_ORDER_SAME = 0,
    PMT_ORDER_DECREASING = 1
} pmt_order_t;

typedef struct pmt_comparator {
    pmt_order_t (*compare)(void*,pmt_universal_t,pmt_universal_t);
    void* arg0;
} pmt_comparator_t;

void pmt_comparator_init(pmt_comparator_t* const restrict comp,
                         pmt_order_t (*compare)(void*,pmt_universal_t,pmt_universal_t),
                         void* arg0);
void pmt_comparator_init_copy(pmt_comparator_t* const restrict dest,
                              pmt_comparator_t* const restrict source);

static inline pmt_order_t
pmt_comparator_compare(pmt_comparator_t const* const restrict comp,
                       pmt_universal_t const a, pmt_universal_t const b)
{
    if( comp->compare ) {
        return (*(comp->compare))(comp->arg0, a, b);
    } else {
        return (a.i64 < b.i64 ? PMT_ORDER_INCREASING : 
                                (a.i64 == b.i64 ? PMT_ORDER_SAME : 
                                                  PMT_ORDER_INCREASING));
    }
}

#endif /* PROMOTE_COMPARATOR_H_ */

