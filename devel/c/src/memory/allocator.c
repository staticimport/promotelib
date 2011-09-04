
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "allocator.h"

/* Init / Uninit */
void pmt_allocator_init_copy(pmt_allocator_t *const restrict allocator,
                             pmt_allocator_t const *const restrict to_copy)
{
    assert(allocator);
    assert(to_copy);
    memcpy(allocator, to_copy, sizeof(pmt_allocator_t));
}

void pmt_allocator_init_standard(pmt_allocator_t *const restrict allocator)
{
    allocator->underlying.pool = 0;
    allocator->type = PMT_ALLOCATOR_STANDARD;
}

void pmt_allocator_init_cache_line(pmt_allocator_t *const restrict allocator)
{
    allocator->underlying.pool = 0;
    allocator->type = PMT_ALLOCATOR_CACHE_LINE;
}

void pmt_allocator_init_with_pool(pmt_allocator_t *const restrict allocator,
                                 pmt_pool_allocator_t *const restrict pool)
{
    allocator->underlying.pool = pool;
    allocator->type = PMT_ALLOCATOR_POOL;
}

void pmt_allocator_init_with_tiered_pool(pmt_allocator_t *const restrict allocator,
                                        pmt_tiered_pool_allocator_t *const restrict tiered_pool)
{
    allocator->underlying.tiered_pool = tiered_pool;
    allocator->type = PMT_ALLOCATOR_TIERED_POOL;
}

void pmt_allocator_init_custom(pmt_allocator_t *const restrict allocator,
                               void* (*alloc)(void*,size_t,bool), void* alloc_arg0,
                               void (*release)(void*,void*), void* release_arg0)
{
    allocator->underlying.custom.alloc = alloc;
    allocator->underlying.custom.alloc_arg0 = alloc_arg0;
    allocator->underlying.custom.release = release;
    allocator->underlying.custom.release_arg0 = release_arg0;
    allocator->type = PMT_ALLOCATOR_CUSTOM;
}

void pmt_allocator_uninit(pmt_allocator_t *const restrict allocator, 
                          bool const uninit_underlying)
{
    if( uninit_underlying ) {
        switch(allocator->type) {
            case PMT_ALLOCATOR_POOL:
                // TODO;
                break;
            case PMT_ALLOCATOR_TIERED_POOL:
                // TODO
                break;
            default:
                break;
        }
    }
}

void *pmt_allocator_alloc(pmt_allocator_t *const restrict allocator,
                          size_t const needed_size,
                          bool const zero_out)
{
    switch(allocator->type) {
        case PMT_ALLOCATOR_POOL:
            // TODO
            return 0;
        case PMT_ALLOCATOR_TIERED_POOL:
            // TODO
            return 0;
        case PMT_ALLOCATOR_STANDARD:
            return zero_out ? calloc(1, needed_size) : malloc(needed_size);
        case PMT_ALLOCATOR_CACHE_LINE:
            return pmt_cache_line_alloc(needed_size, zero_out);
        case PMT_ALLOCATOR_CUSTOM:
            return (*(allocator->underlying.custom.alloc))(allocator->underlying.custom.alloc_arg0,
                                                           needed_size, zero_out);
        default:
            return 0;
    }
}

int pmt_allocator_release(pmt_allocator_t *const restrict allocator,
                          void *const restrict alloced)
{
    switch(allocator->type) {
        case PMT_ALLOCATOR_POOL:
            // TODO
            return 0;
        case PMT_ALLOCATOR_TIERED_POOL:
            // TODO
            return 0;
        case PMT_ALLOCATOR_STANDARD:
            free(alloced);
            return 0;
        case PMT_ALLOCATOR_CACHE_LINE:
            pmt_cache_line_release(alloced);
            return 0;
        case PMT_ALLOCATOR_CUSTOM:
            (*(allocator->underlying.custom.release))(allocator->underlying.custom.release_arg0, 
                                                      alloced);            
            return 0;
        default:
            return -1;
    }
}

