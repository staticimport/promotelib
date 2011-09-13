
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "allocator.h"
#include "cache_line_allocator.h"

/* Init / Uninit */
/*void pro_allocator_init_copy(pro_allocator_t *const restrict allocator,
                             pro_allocator_t const *const restrict to_copy)
{
    assert(allocator);
    assert(to_copy);
    memcpy(allocator, to_copy, sizeof(pro_allocator_t));
}*/

void pro_allocator_init_standard(pro_allocator_t *const restrict allocator)
{
  allocator->type = PRO_ALLOCATOR_STANDARD;
}

void pro_allocator_init_cache_line(pro_allocator_t *const restrict allocator)
{
  allocator->type = PRO_ALLOCATOR_CACHE_LINE;
}

void pro_allocator_init_pool(pro_allocator_t *const restrict allocator,
                             size_t const unit_size, size_t const init_capacity)
{
  pro_pool_allocator_init(&(allocator->underlying.pool), unit_size,
                          init_capacity);
  allocator->type = PRO_ALLOCATOR_POOL;
}

void pro_allocator_init_custom(pro_allocator_t *const restrict allocator,
                               void* (*alloc)(void*,size_t,bool), void* alloc_arg0,
                               void (*release)(void*,void*), void* release_arg0)
{
  allocator->underlying.custom.alloc = alloc;
  allocator->underlying.custom.alloc_arg0 = alloc_arg0;
  allocator->underlying.custom.release = release;
  allocator->underlying.custom.release_arg0 = release_arg0;
  allocator->type = PRO_ALLOCATOR_CUSTOM;
}

void pro_allocator_uninit(pro_allocator_t *const restrict allocator)
{
  switch(allocator->type) {
    case PRO_ALLOCATOR_POOL:
      pro_pool_allocator_uninit(&(allocator->underlying.pool));
      break;
    default:
      break;
  }
}

void *pro_allocator_alloc(pro_allocator_t *const restrict allocator,
                          size_t const needed_size,
                          bool const zero_out)
{
  switch(allocator->type) {
    case PRO_ALLOCATOR_POOL:
      return pro_pool_allocator_alloc(&(allocator->underlying.pool),
                                      needed_size, zero_out);
    case PRO_ALLOCATOR_STANDARD:
      return zero_out ? calloc(1, needed_size) : malloc(needed_size);
    case PRO_ALLOCATOR_CACHE_LINE:
      return pro_cache_line_alloc(needed_size, zero_out);
    case PRO_ALLOCATOR_CUSTOM:
      return (*(allocator->underlying.custom.alloc))(
        allocator->underlying.custom.alloc_arg0, needed_size, zero_out);
    default:
      assert(false);
      return 0;
  }
}

void pro_allocator_release(pro_allocator_t *const restrict allocator,
                           void *const restrict alloced)
{
  switch(allocator->type) {
    case PRO_ALLOCATOR_POOL:
      pro_pool_allocator_release(&(allocator->underlying.pool), alloced);
      break;
    case PRO_ALLOCATOR_STANDARD:
      free(alloced);
      break;
    case PRO_ALLOCATOR_CACHE_LINE:
      pro_cache_line_release(alloced);
      break;
    case PRO_ALLOCATOR_CUSTOM:
      (*(allocator->underlying.custom.release))(
        allocator->underlying.custom.release_arg0, alloced);
      break;
    default:
      assert(false);
      break;
  }
}

