#ifndef PROMOTE_ARRAY_H
#define PROMOTE_ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct pmt_array {
    pmt_universal_t* data;
    pmt_universal_t* end;
    size_t length;
} pmt_array_t;

// Init + Uninit
bool
pmt_array_init(pmt_array_t *const restrict array,
               size_t const length);

bool
pmt_array_uninit(pmt_array_t *const restrict array);

// Observe
static inline pmt_universal_t const*
pmt_array_const_begin(pmt_array_t const *const restrict array) {
    return array->data;
}

static inline pmt_universal_t const*
pmt_array_const_end(pmt_array_t const *const restrict array) {
    return array->end;
}

static inline pmt_universal_t
pmt_array_get(pmt_array_t const *const restrict array,
              size_t const index)
{
    return array->data[index];
}

static inline size_t
pmt_array_length(pmt_array_t const *const restrict array) {
    return array->length;
}

// Modify
static inline void
pmt_array_resize(pmt_array_t *const restrict array,
                 size_t const length)
{
    array->data = realloc(array->data, array->length * sizeof(pmt_universal_t));
    array->end = array->data + length;
    array->length = length;
}

static inline void
pmt_array_reserve(pmt_array_t *const restrict array,
                  size_t const length)
{
    if( length > array->length ) {
        pmt_array_resize(array, length);
    }
}

static inline void
pmt_array_set(pmt_array_t *const restrict array,
              size_t const index,
              pmt_universal_t const value)
{
    assert(array->length > index);
    array->data[index] = value;
}

#endif /* PROMOTE_ARRAY_H */

