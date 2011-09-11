
#include <assert.h>

#include "hash.h"

void
pro_hash_functor_init(pro_hash_functor_t *const restrict functor,
                      pro_hash_t (*function)(void*,pro_item_t const),
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
pro_hash_functor_uninit(pro_hash_functor_t *const restrict functor)
{
    assert(functor);
    if( functor->uninit ) {
        (*(functor->uninit))(functor->arg0);
    }
}

