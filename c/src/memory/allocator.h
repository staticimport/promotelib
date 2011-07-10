#ifndef PROMOTE_ALLOCATOR_H_
#define PROMOTE_ALLOCATOR_H_

#include <stdbool.h>
#include <stdlib.h>

#include "cache_line_allocator.h"
#include "pool_allocator.h"
#include "tiered_pool_allocator.h"

enum pmt_allocator_type {
    PMT_ALLOCATOR_STANDARD,
    PMT_ALLOCATOR_CACHE_LINE,
    PMT_ALLOCATOR_POOL,
    PMT_ALLOCATOR_TIERED_POOL,
    PMT_ALLOCATOR_CUSTOM
};

typedef struct pmt_allocator {
    union {
        pmt_pool_allocator_t *pool;
        pmt_tiered_pool_allocator_t *tiered_pool;
        struct {
            void* (*alloc)(void*,size_t,bool);
            void* alloc_arg0;
            void (*release)(void*,void*);
            void* release_arg0;
        } custom;
    } underlying;
    enum pmt_allocator_type type;
} pmt_allocator_t;


/* Init / Uninit */
void pmt_allocator_init_copy(pmt_allocator_t *const restrict allocator,
                             pmt_allocator_t const *const restrict to_copy);
void pmt_allocator_init_standard(pmt_allocator_t *const restrict allocator);
void pmt_allocator_init_cache_line(pmt_allocator_t *const restrict allocator);
void pmt_allocator_init_with_pool(pmt_allocator_t *const restrict allocator,
                                  pmt_pool_allocator_t *const restrict pool);
void pmt_allocator_init_with_tiered_pool(pmt_allocator_t *const restrict allocator,
                                        pmt_tiered_pool_allocator_t *const restrict tiered_pool);
void pmt_allocator_init_custom(pmt_allocator_t *const restrict allocator,
                               void* (*alloc)(void*,size_t,bool), void* alloc_arg0,
                               void (*release)(void*,void*), void* release_arg0);

void pmt_allocator_uninit(pmt_allocator_t *const restrict allocator, bool uninit_underlying);


/* Alloc / Release */
void *pmt_allocator_alloc(pmt_allocator_t *const restrict allocator,
                          size_t const needed_size,
                          bool const zero_out);
int pmt_allocator_release(pmt_allocator_t *const restrict allocator,
                          void *const restrict alloced);

#endif /* PROMOTE_ALLOCATOR_H_ */

