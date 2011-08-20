#ifndef PROMOTE_VECTOR_H
#define PROMOTE_VECTOR_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "universal.h"

static const size_t PROMOTE_VECTOR_INVALID_INDEX = -1;

typedef struct pmt_vector {
  pmt_universal_t* array;
  size_t capacity;
  size_t size;
} pmt_vector_t;

typedef struct pmt_vector_iterator {
  pmt_universal_t const* next;
  pmt_universal_t const* end;
} pmt_vector_iterator_t;

/* Init / Uninit */
void
pmt_vector_init(pmt_vector_t* const restrict vector,
                size_t const init_capacity);

void
pmt_vector_uninit(pmt_vector_t* const restrict vector);

/* Observe */
static inline pmt_universal_t
pmt_vector_at_index(pmt_vector_t const* const restrict vector,
                    size_t const index);

static inline pmt_universal_t
pmt_vector_back(pmt_vector_t const* const restrict vector);

static inline pmt_universal_t
pmt_vector_front(pmt_vector_t const* const restrict vector);

static inline size_t
pmt_vector_index_of(pmt_vector_t const* const restrict vector,
                    pmt_universal_t const item);

static inline bool
pmt_vector_is_empty(pmt_vector_t const* const restrict vector);

static inline size_t
pmt_vector_last_index_of(pmt_vector_t const* const restrict vector,
                         pmt_universal_t const item);

static inline size_t
pmt_vector_size(pmt_vector_t const* const restrict vector);

/* Modify */
static inline void
pmt_vector_add_at_index(pmt_vector_t* const restrict vector,
                        pmt_universal_t const item,
                        size_t const index);

static inline void
pmt_vector_add_back(pmt_vector_t* const restrict vector,
                    pmt_universal_t const item);

bool
pmt_vector_ensure_capacity(pmt_vector_t* const restrict vector,
                           size_t const capacity);

static inline void
pmt_vector_remove_at_index(pmt_vector_t* const restrict vector,
                           size_t const index);

static inline void
pmt_vector_remove_back(pmt_vector_t* const restrict vector);

static inline bool
pmt_vector_remove(pmt_vector_t* const restrict vector,
                  pmt_universal_t const item);

static inline bool
pmt_vector_remove_last(pmt_vector_t* const restrict vector,
                       pmt_universal_t const item);

/* Iterator */
static inline void
pmt_vector_iterator_init(pmt_vector_t const* const restrict vector,
                         pmt_vector_iterator_t* const restrict iter);

static inline bool
pmt_vector_iterator_has_next(pmt_vector_iterator_t const* const restrict iter);

static inline pmt_universal_t
pmt_vector_iterator_next(pmt_vector_iterator_t* const restrict iter);

/***
 * Inline Implemenations
 ***/
static inline pmt_universal_t
pmt_vector_at_index(pmt_vector_t const* const restrict vector,
                    size_t const index)
{
  assert(index < vector->size);
  return vector->array[index];
}

static inline pmt_universal_t
pmt_vector_back(pmt_vector_t const* const restrict vector)
{
  assert(vector->size);
  return vector->array[vector->size-1];
}

static inline pmt_universal_t
pmt_vector_front(pmt_vector_t const* const restrict vector)
{
  assert(vector->size);
  return vector->array[0];
}

static inline size_t
pmt_vector_index_of(pmt_vector_t const* const restrict vector,
                    pmt_universal_t const item)
{
  pmt_universal_t* const end = vector->array + vector->size;
  for(pmt_universal_t* entry = vector->array; entry != end; ++entry) {
    if( pmt_universal_are_equal(*entry, item) ) {
      return entry - vector->array;
    }
  }
  return PROMOTE_VECTOR_INVALID_INDEX;
}

static inline bool
pmt_vector_is_empty(pmt_vector_t const* const restrict vector)
{
  return vector->size == 0;
}

static inline size_t
pmt_vector_last_index_of(pmt_vector_t const* const restrict vector,
                         pmt_universal_t const item)
{
  pmt_universal_t* const front = vector->array;
  for(pmt_universal_t* entry = front + vector->size - 1; entry >= front; --entry) {
    if( pmt_universal_are_equal(*entry, item) ) {
      return entry - front;
    }
  }
  return PROMOTE_VECTOR_INVALID_INDEX;
}

static inline size_t
pmt_vector_size(pmt_vector_t const* const restrict vector)
{
  return vector->size;
}

/* Modify */
static inline void
pmt_vector_add_at_index(pmt_vector_t* const restrict vector,
                        pmt_universal_t const item,
                        size_t const index)
{
  if( vector->size == vector->capacity ) {
    bool expanded = pmt_vector_ensure_capacity(vector, vector->capacity << 1);
    assert(expanded);
  }
  memmove(vector->array + index + 1, vector->array + index, 
          (vector->size++ - index) * sizeof(pmt_universal_t));
  vector->array[index] = item;
}

static inline void
pmt_vector_add_back(pmt_vector_t* const restrict vector,
                    pmt_universal_t const item)
{
  if( vector->size == vector->capacity) {
    bool expanded = pmt_vector_ensure_capacity(vector, vector->capacity << 1);
    assert(expanded);
  }
  vector->array[vector->size++] = item;
}

static inline void
pmt_vector_remove_at_index(pmt_vector_t* const restrict vector,
                           size_t const index)
{
  assert(vector->size > index);
  memmove(vector->array + index, vector->array + index + 1, 
          (--(vector->size) - index) * sizeof(pmt_universal_t));
}

static inline void
pmt_vector_remove_back(pmt_vector_t* const restrict vector)
{
  assert(vector->size);
  --(vector->size);
}

static inline bool
pmt_vector_remove(pmt_vector_t* const restrict vector,
                  pmt_universal_t const item)
{
  size_t const index = pmt_vector_index_of(vector, item);
  if( index != PROMOTE_VECTOR_INVALID_INDEX ) {
    pmt_vector_remove_at_index(vector, index);
    return true;
  } else {
    return false;
  }
}

static inline bool
pmt_vector_remove_last(pmt_vector_t* const restrict vector,
                       pmt_universal_t const item)
{
  size_t const index = pmt_vector_last_index_of(vector, item);
  if( index != PROMOTE_VECTOR_INVALID_INDEX ) {
    pmt_vector_remove_at_index(vector, index);
    return true;
  } else {
    return false;
  }
}

/* Iterator */
static inline void
pmt_vector_iterator_init(pmt_vector_t const* const restrict vector,
                         pmt_vector_iterator_t* const restrict iter)
{
  iter->next = vector->array;
  iter->end = vector->array + vector->size;
}

static inline bool
pmt_vector_iterator_has_next(pmt_vector_iterator_t const* const restrict iter)
{
  return iter->next != iter->end;
}

static inline pmt_universal_t
pmt_vector_iterator_next(pmt_vector_iterator_t* const restrict iter)
{
  assert(iter->next != iter->end);
  return *(iter->next)++;
}

#endif /* PROMOTE_VECTOR_H */

