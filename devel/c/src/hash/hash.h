#ifndef PROMOTE_HASH_H_
#define PROMOTE_HASH_H_

#include <stdint.h>

#include "universal.h"

#ifndef __LP64__
typedef uint64_t pmt_hash_t;
pmt_hash_t const PMT_HASH_MAX = UINT64_MAX;
#else
typedef uint32_t pmt_hash_t;
pmt_hash_t const PMT_HASH_MAX = UINT32_MAX;
#endif
pmt_hash_t const PMT_HASH_MIN = 0;

typedef struct pmt_hash_functor {
    pmt_hash_t (*function)(void*,pmt_universal_t);
    void (*uninit)(void*);
    void* arg0;
} pmt_hash_functor_t;


void
pmt_hash_functor_init(pmt_hash_functor_t *const restrict functor,
                      pmt_hash_t (*function)(void*,pmt_universal_t),
                      void (*uninit)(void*),
                      void* arg0);

void
pmt_hash_functor_uninit(pmt_hash_functor_t *const restrict functor);

static inline pmt_hash_t
pmt_hash_functor_hash(pmt_hash_functor_t *const restrict functor,
                      pmt_universal_t const value)
{
    return (*(functor->function))(functor->arg0, value);
}

#endif /* PROMOTE_HASH_H_ */

