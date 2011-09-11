#ifndef PROMOTE_COMPARATOR_H_
#define PROMOTE_COMPARATOR_H_

#include "item.h"

typedef enum pro_order {
    PRO_ORDER_INCREASING = -1,
    PRO_ORDER_SAME = 0,
    PRO_ORDER_DECREASING = 1
} pro_order_t;

typedef struct pro_comparator {
    pro_order_t (*compare)(void*,pro_item_t const,pro_item_t const);
    void* arg0;
} pro_comparator_t;

/**
 * Prototypes
 **/
void pro_comparator_init(pro_comparator_t* const restrict comp,
                         pro_order_t (*compare)(void*,pro_item_t const,pro_item_t const),
                         void* arg0);
void pro_comparator_init_copy(pro_comparator_t* const restrict dest,
                              pro_comparator_t* const restrict source);

static inline pro_order_t
pro_comparator_compare(pro_comparator_t const* const restrict comp,
                       pro_item_t const a, pro_item_t const b);

/**
 * Inline Implementations
 **/
static inline pro_order_t
pro_comparator_compare(pro_comparator_t const* const restrict comp,
                       pro_item_t const a, pro_item_t const b)
{
  return (*(comp->compare))(comp->arg0, a, b);
}

#endif /* PROMOTE_COMPARATOR_H_ */

