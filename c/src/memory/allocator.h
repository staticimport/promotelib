#ifndef PROMOTE_ALLOCATOR_H
#define PROMOTE_ALLOCATOR_H

#include <stdbool.h>
#include <stdlib.h>

#include "pool_allocator.h"

enum pro_allocator_type {
    PRO_ALLOCATOR_STANDARD,
    PRO_ALLOCATOR_CACHE_LINE,
    PRO_ALLOCATOR_POOL,
    PRO_ALLOCATOR_CUSTOM
};

typedef struct pro_allocator {
    union {
        pro_pool_allocator_t pool;
        struct {
            void* (*alloc)(void*,size_t,bool);
            void* alloc_arg0;
            void (*release)(void*,void*);
            void* release_arg0;
        } custom;
    } underlying;
    enum pro_allocator_type type;
} pro_allocator_t;


/* Init / Uninit */
//void pro_allocator_init_copy(pro_allocator_t *const restrict allocator,
//                             pro_allocator_t const *const restrict to_copy);
void pro_allocator_init_standard(pro_allocator_t *const restrict allocator);
void pro_allocator_init_cache_line(pro_allocator_t *const restrict allocator);
void pro_allocator_init_pool(pro_allocator_t *const restrict allocator,
                             size_t const unit_size, size_t const init_capacity);
void pro_allocator_init_custom(pro_allocator_t *const restrict allocator,
                               void* (*alloc)(void*,size_t,bool), void* alloc_arg0,
                               void (*release)(void*,void*), void* release_arg0);

void pro_allocator_uninit(pro_allocator_t *const restrict allocator);


/* Alloc / Release */
void *pro_allocator_alloc(pro_allocator_t *const restrict allocator,
                          size_t const needed_size,
                          bool const zero_out);
void pro_allocator_release(pro_allocator_t *const restrict allocator,
                           void *const restrict alloced);

#endif /* PROMOTE_ALLOCATOR_H */

