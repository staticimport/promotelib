#ifndef PROMOTE_BINARY_HEAP_H_
#define PROMOTE_BINARY_HEAP_H_

#include <stdbool.h>

#include "comparator.h"
#include "universal.h"

typedef struct pmt_binheap {
    pmt_comparator_t comparator;
    pmt_universal_t* array;
    size_t capacity;
    size_t size;
    bool min_heap;
} pmt_binheap_t;

/* Init / Uninit */
bool pmt_binheap_init(pmt_binheap_t *const restrict heap,
                      pmt_comparator_t* const restrict comparator_to_copy,
                      size_t const init_capacity,
                      bool const min_heap);
bool pmt_binheap_uninit(pmt_binheap_t *const restrict heap);

/* Observe */
static inline bool
pmt_binheap_is_empty(pmt_binheap_t const* const restrict heap)
{
    return heap->size == 0;
}

static inline pmt_universal_t
pmt_binheap_peek(pmt_binheap_t const* const restrict heap)
{
    return heap->array[1];
}

static inline size_t
pmt_binheap_size(pmt_binheap_t const* const restrict heap)
{
    return heap->size;
}

/* Modify */
void pmt_binheap_add(pmt_binheap_t* const restrict heap,
                     pmt_universal_t const item);

pmt_universal_t pmt_binheap_pop(pmt_binheap_t* const restrict heap);

#endif /* PROMOTE_BINARY_HEAP_H_ */

