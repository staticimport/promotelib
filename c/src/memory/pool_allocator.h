#ifndef PROMOTE_POOL_ALLOCATOR_H
#define PROMOTE_POOL_ALLOCATOR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct pro_pool_allocator {
  size_t unit_size;
} pro_pool_allocator_t;

/* Init/Uninit */
void pro_pool_allocator_init(pro_pool_allocator_t* const restrict pool,
                             size_t const unit_size, size_t const init_capacity);
void pro_pool_allocator_uninit(pro_pool_allocator_t* const restrict pool);

/* Alloc/Release */
void* pro_pool_allocator_alloc(pro_pool_allocator_t* const restrict pool,
                               size_t const size, bool const zero_out);
void pro_pool_allocator_release(pro_pool_allocator_t* const restrict pool,
                                void* const restrict alloced);

#endif /* PROMOTE_POOL_ALLOCATOR_H */

