
#include <assert.h>

#include "hash.h"

void
pmt_hash_functor_init(pmt_hash_functor_t *const restrict functor,
                      pmt_hash_t (*function)(void*,pmt_universal_t),
                      void (*uninit)(void*),
                      void* arg0)
{
    assert(functor);
    assert(function);
    functor->function = function;
    functor->uninit = uninit;
    functor->arg0 = arg0;
}

void
pmt_hash_functor_uninit(pmt_hash_functor_t *const restrict functor)
{
    assert(functor);
    if( functor->uninit ) {
        (*(functor->uninit))(functor->arg0);
    }
}

