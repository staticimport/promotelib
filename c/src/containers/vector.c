
#include "vector.h"

void
pro_vector_init(pro_vector_t* const restrict vector,
                size_t const init_capacity)
{
  vector->array = malloc(init_capacity * sizeof(pro_item_t));
  vector->capacity = init_capacity;
  vector->size = 0;
}

void
pro_vector_uninit(pro_vector_t* const restrict vector)
{
  free(vector->array);
  vector->array = 0;
}

bool
pro_vector_ensure_capacity(pro_vector_t* const restrict vector,
                           size_t const capacity)
{
  if( vector->capacity >= capacity) return true;

  vector->array = realloc(vector->array, capacity * sizeof(pro_item_t));
  vector->capacity = capacity;

  assert(vector->array);
  return vector->array != 0;
}

