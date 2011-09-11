#ifndef PROMOTE_HASH_H
#define PROMOTE_HASH_H

#include <stdint.h>

#include "item.h"

typedef uint32_t pro_hash_t;
static pro_hash_t const PRO_HASH_MAX = UINT32_MAX;
static pro_hash_t const PRO_HASH_MIN = 0;

typedef struct pro_hash_functor {
    pro_hash_t (*function)(void*,pro_item_t const);
    void (*uninit)(void*);
    void* arg0;
} pro_hash_functor_t;

/**
 * Prototypes
 **/
void
pro_hash_functor_init(pro_hash_functor_t *const restrict functor,
                      pro_hash_t (*function)(void*,pro_item_t const),
                      void (*uninit)(void*),
                      void* arg0);

void
pmt_hash_functor_uninit(pro_hash_functor_t *const restrict functor);

static inline pro_hash_t
pro_hash_functor_hash(pro_hash_functor_t const *const restrict functor,
                      pro_item_t const value);

/**
 * Inline implemenations
 **/
static inline pro_hash_t
pro_hash_functor_hash(pro_hash_functor_t const* const restrict functor,
                      pro_item_t const value)
{
    return (*(functor->function))(functor->arg0, value);
}

#endif /* PROMOTE_HASH_H_ */

