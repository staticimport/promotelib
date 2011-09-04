
#include <assert.h>
#include <iso646.h>
#include <stdlib.h>

#include "binary_heap.h"

bool pmt_binheap_init(pmt_binheap_t *const restrict heap,
                      pmt_comparator_t* const restrict comparator_to_copy,
                      size_t const init_capacity,
                      bool const min_heap)
{
    assert(heap);
    assert(comparator_to_copy);
    if( heap and comparator_to_copy ) {
        pmt_comparator_init_copy(&(heap->comparator), comparator_to_copy);
        heap->array = malloc((init_capacity+1) * sizeof(pmt_universal_t));
        assert(heap->array);
        if( heap->array ) {
            heap->capacity = init_capacity;
            heap->size = 0;
            heap->min_heap = min_heap;
            return true;
        }
    }
    return false;
}

bool pmt_binheap_uninit(pmt_binheap_t *const restrict heap)
{
    assert(heap);
    if( heap ) {
        free(heap->array);
        return true;
    } else {
        return false;
    }
}

static inline bool
pmt_binheap_is_order_correct(pmt_binheap_t* const restrict heap,
                             pmt_universal_t const a,
                             pmt_universal_t const b)
{
    return heap->min_heap xor
           (pmt_comparator_compare(&(heap->comparator), a, b) == PMT_ORDER_DECREASING);
}

void pmt_binheap_add(pmt_binheap_t* const restrict heap,
                     pmt_universal_t const item)
{
    // Expand if necessary
    if( heap->size == heap->capacity ) {
        heap->capacity <<= 1;
        heap->array = realloc(heap->array, (heap->capacity+1) * sizeof(pmt_universal_t));
    }

    // Insert at bottom and rotate up
    size_t index = ++(heap->size);
    size_t parent_index = index >> 1;
    while( parent_index ) {
        if( !pmt_binheap_is_order_correct(heap, heap->array[parent_index], item) ) {
            heap->array[index] = heap->array[parent_index];
            parent_index = (index = parent_index) >> 1;
        } else {
            break;
        }
    }
    heap->array[index] = item;
}

pmt_universal_t pmt_binheap_pop(pmt_binheap_t* const restrict heap)
{
    pmt_universal_t const top = heap->array[1];

    // Take value at end of array and push down from top
    pmt_universal_t const last = heap->array[(heap->size)--];
    size_t index = 1;
    size_t left = index << 1;
    size_t right = left + 1;
    while( left <= heap->size ) {
        size_t swap = index;
        if( left != heap->size ) {
            // Right is valid too
            if( !pmt_binheap_is_order_correct(heap, last, heap->array[left]) ) {
                swap = pmt_binheap_is_order_correct(heap, heap->array[left], heap->array[right]) ?
                       left : right;
            } else if( !pmt_binheap_is_order_correct(heap, last, heap->array[right]) ) {
                swap = right;
            }
        } else if( !pmt_binheap_is_order_correct(heap, last, heap->array[left]) ) {
            swap = left;
        } else {
            break;
        }
    
        heap->array[index] = heap->array[swap];
        index = swap;
        right = (left = (index << 1)) + 1;
    }
    heap->array[index] = last;

    return top;
}

