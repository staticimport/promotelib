
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cache_line_allocator.h"

#define PRO_CACHE_LINE_SIZE_LOG2 (6)
static const size_t PRO_CACHE_LINE_SIZE = 1 << PRO_CACHE_LINE_SIZE_LOG2;
static const size_t PRO_CACHE_LINE_MASK = (1 << PRO_CACHE_LINE_SIZE_LOG2) - 1;

void*
pro_cache_line_alloc(size_t const size, bool const zero_out)
{
  // Smallest multiple of 64 >= size, plus 1
  size_t const size_div_line = size >> PRO_CACHE_LINE_SIZE_LOG2;
  size_t const size_mod_line = size & PRO_CACHE_LINE_MASK;
  size_t const extra_lines = size_mod_line ? 2 : 1;
  size_t const num_lines = size_div_line + extra_lines;

  // Alloc
  uint8_t* const alloced = malloc(num_lines << PRO_CACHE_LINE_SIZE_LOG2);
  size_t const to_next_line = PRO_CACHE_LINE_SIZE - (((size_t)alloced) & PRO_CACHE_LINE_MASK);
  uint8_t const offset = (uint8_t)(to_next_line ? to_next_line : PRO_CACHE_LINE_SIZE);
  uint8_t* const to_return = alloced + offset;

  // Insert offset
  *(to_return - 1) = offset;

  // Zero out
  if( zero_out ) {
    memset(to_return, 0, size);
  }

  // Return
  return to_return;
}

void
pro_cache_line_release(void* alloced)
{
  uint8_t* const alloced_bytes = (uint8_t*)alloced;
  uint8_t const offset = *(alloced_bytes - 1);
  free(alloced_bytes - offset);
}

#undef PRO_CACHE_LINE_SIZE_LOG2

