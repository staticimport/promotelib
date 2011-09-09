#ifndef PROMOTE_VECTOR_H
#define PROMOTE_VECTOR_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"

static const size_t PROMOTE_VECTOR_INVALID_INDEX = -1;

typedef struct pro_vector {
  pro_item_t* array;
  size_t capacity;
  size_t size;
} pro_vector_t;

typedef struct pro_vector_iterator {
  pro_item_t const* next;
  pro_item_t const* end;
} pro_vector_iterator_t;

/* Init / Uninit */
void
pro_vector_init(pro_vector_t* const restrict vector,
                size_t const init_capacity);

void
pro_vector_uninit(pro_vector_t* const restrict vector);

/* Observe */
static inline pro_item_t
pro_vector_at_index(pro_vector_t const* const restrict vector,
                    size_t const index);

static inline pro_item_t
pro_vector_back(pro_vector_t const* const restrict vector);

static inline pro_item_t
pro_vector_front(pro_vector_t const* const restrict vector);

static inline size_t
pro_vector_index_of(pro_vector_t const* const restrict vector,
                    pro_item_t const item);

static inline bool
pro_vector_is_empty(pro_vector_t const* const restrict vector);

static inline size_t
pro_vector_last_index_of(pro_vector_t const* const restrict vector,
                         pro_item_t const item);

static inline size_t
pro_vector_size(pro_vector_t const* const restrict vector);

/* Modify */
static inline void
pro_vector_add_at_index(pro_vector_t* const restrict vector,
                        pro_item_t const item,
                        size_t const index);

static inline void
pro_vector_add_back(pro_vector_t* const restrict vector,
                    pro_item_t const item);

bool
pro_vector_ensure_capacity(pro_vector_t* const restrict vector,
                           size_t const capacity);

static inline void
pro_vector_remove_at_index(pro_vector_t* const restrict vector,
                           size_t const index);

static inline void
pro_vector_remove_back(pro_vector_t* const restrict vector);

static inline bool
pro_vector_remove(pro_vector_t* const restrict vector,
                  pro_item_t const item);

static inline bool
pro_vector_remove_last(pro_vector_t* const restrict vector,
                       pro_item_t const item);

/* Iterator */
static inline void
pro_vector_iterator_init(pro_vector_t const* const restrict vector,
                         pro_vector_iterator_t* const restrict iter);

static inline bool
pro_vector_iterator_has_next(pro_vector_iterator_t const* const restrict iter);

static inline pro_item_t
pro_vector_iterator_next(pro_vector_iterator_t* const restrict iter);

/***
 * Inline Implemenations
 ***/
static inline pro_item_t
pro_vector_at_index(pro_vector_t const* const restrict vector,
                    size_t const index)
{
  assert(index < vector->size);
  return vector->array[index];
}

static inline pro_item_t
pro_vector_back(pro_vector_t const* const restrict vector)
{
  assert(vector->size);
  return vector->array[vector->size-1];
}

static inline pro_item_t
pro_vector_front(pro_vector_t const* const restrict vector)
{
  assert(vector->size);
  return vector->array[0];
}

static inline size_t
pro_vector_index_of(pro_vector_t const* const restrict vector,
                    pro_item_t const item)
{
  pro_item_t* const end = vector->array + vector->size;
  for(pro_item_t* entry = vector->array; entry != end; ++entry) {
    if( pro_item_are_equal(*entry, item) ) {
      return entry - vector->array;
    }
  }
  return PROMOTE_VECTOR_INVALID_INDEX;
}

static inline bool
pro_vector_is_empty(pro_vector_t const* const restrict vector)
{
  return vector->size == 0;
}

static inline size_t
pro_vector_last_index_of(pro_vector_t const* const restrict vector,
                         pro_item_t const item)
{
  pro_item_t* const front = vector->array;
  for(pro_item_t* entry = front + vector->size - 1; entry >= front; --entry) {
    if( pro_item_are_equal(*entry, item) ) {
      return entry - front;
    }
  }
  return PROMOTE_VECTOR_INVALID_INDEX;
}

static inline size_t
pro_vector_size(pro_vector_t const* const restrict vector)
{
  return vector->size;
}

/* Modify */
static inline void
pro_vector_add_at_index(pro_vector_t* const restrict vector,
                        pro_item_t const item,
                        size_t const index)
{
  if( vector->size == vector->capacity ) {
    bool expanded = pro_vector_ensure_capacity(vector, vector->capacity << 1);
    assert(expanded);
  }
  memmove(vector->array + index + 1, vector->array + index, 
          (vector->size++ - index) * sizeof(pro_item_t));
  vector->array[index] = item;
}

static inline void
pro_vector_add_back(pro_vector_t* const restrict vector,
                    pro_item_t const item)
{
  if( vector->size == vector->capacity) {
    bool expanded = pro_vector_ensure_capacity(vector, vector->capacity << 1);
    assert(expanded);
  }
  vector->array[vector->size++] = item;
}

static inline void
pro_vector_remove_at_index(pro_vector_t* const restrict vector,
                           size_t const index)
{
  assert(vector->size > index);
  memmove(vector->array + index, vector->array + index + 1, 
          (--(vector->size) - index) * sizeof(pro_item_t));
}

static inline void
pro_vector_remove_back(pro_vector_t* const restrict vector)
{
  assert(vector->size);
  --(vector->size);
}

static inline bool
pro_vector_remove(pro_vector_t* const restrict vector,
                  pro_item_t const item)
{
  size_t const index = pro_vector_index_of(vector, item);
  if( index != PROMOTE_VECTOR_INVALID_INDEX ) {
    pro_vector_remove_at_index(vector, index);
    return true;
  } else {
    return false;
  }
}

static inline bool
pro_vector_remove_last(pro_vector_t* const restrict vector,
                       pro_item_t const item)
{
  size_t const index = pro_vector_last_index_of(vector, item);
  if( index != PROMOTE_VECTOR_INVALID_INDEX ) {
    pro_vector_remove_at_index(vector, index);
    return true;
  } else {
    return false;
  }
}

/* Iterator */
static inline void
pro_vector_iterator_init(pro_vector_t const* const restrict vector,
                         pro_vector_iterator_t* const restrict iter)
{
  iter->next = vector->array;
  iter->end = vector->array + vector->size;
}

static inline bool
pro_vector_iterator_has_next(pro_vector_iterator_t const* const restrict iter)
{
  return iter->next != iter->end;
}

static inline pro_item_t
pro_vector_iterator_next(pro_vector_iterator_t* const restrict iter)
{
  assert(iter->next != iter->end);
  return *(iter->next)++;
}

#endif /* PROMOTE_VECTOR_H */

