
#include "vector.h"

void
pmt_vector_init(pmt_vector_t* const restrict vector,
                size_t const init_capacity)
{
  vector->array = malloc(init_capacity * sizeof(pmt_universal_t));
  vector->capacity = init_capacity;
  vector->size = 0;
}

void
pmt_vector_uninit(pmt_vector_t* const restrict vector)
{
  free(vector->array);
  vector->array = 0;
}

bool
pmt_vector_ensure_capacity(pmt_vector_t* const restrict vector,
                           size_t const capacity)
{
  if( vector->capacity >= capacity) return true;

  vector->array = realloc(vector->array, capacity * sizeof(pmt_universal_t));
  vector->capacity = capacity;

  assert(vector->array);
  return vector->array != 0;
}

