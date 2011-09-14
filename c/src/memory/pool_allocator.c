
#include <assert.h>
#include <stdlib.h>

#include "pool_allocator.h"

void pro_pool_allocator_init(pro_pool_allocator_t* const restrict pool,
                             size_t const unit_size, size_t const init_capacity)
{
  pool->unit_size = unit_size;
}

void pro_pool_allocator_uninit(pro_pool_allocator_t* const restrict pool)
{
}

void* pro_pool_allocator_alloc(pro_pool_allocator_t* const restrict pool,
                               size_t const size, bool const zero_out)
{
  assert(size <= pool->unit_size);
  return zero_out ? calloc(1, size) : malloc(size);
}

void pro_pool_allocator_release(pro_pool_allocator_t* const restrict pool,
                                void* const restrict alloced)
{
  free(alloced);
}

