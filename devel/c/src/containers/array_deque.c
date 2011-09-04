
#include <stdlib.h>
#include <string.h>

#include "array_deque.h"
#include "base2.h"

void pmt_arraydeque_init(pmt_arraydeque_t* const restrict deque,
                        size_t const min_init_capacity)
{
    assert(deque);
    assert(min_init_capacity);

    deque->capacity_log2 = (size_t)pmt_log2_ceil64((uint64_t)min_init_capacity);
    deque->capacity = 1 << deque->capacity_log2;
    deque->mask = deque->capacity - 1;
    deque->size = 0;
    deque->front = 0;
    deque->array = malloc(sizeof(pmt_universal_t) << deque->capacity_log2);
}

void pmt_arraydeque_uninit(pmt_arraydeque_t* const restrict deque)
{
    free(deque->array);
}

bool
pmt_arraydeque_ensure_capacity(pmt_arraydeque_t* const restrict deque,
                              size_t const min_capacity)
{
    if( deque->capacity < min_capacity ) {
        size_t const new_capacity_log2 = (size_t)pmt_log2_ceil64((uint64_t)min_capacity);
        size_t const new_capacity = 1 << new_capacity_log2;
        pmt_universal_t* new_array = realloc(deque->array, sizeof(pmt_universal_t) << new_capacity_log2);
        size_t const frontToEndCount = deque->capacity - deque->front;
        if( frontToEndCount < deque->size ) {
            memcpy(new_array + deque->capacity, new_array, (deque->size - frontToEndCount) * sizeof(pmt_universal_t));
        }
        deque->array = new_array;
        deque->capacity = new_capacity;
        deque->capacity_log2 = new_capacity_log2;
        deque->mask = deque->capacity - 1;
    }
    return true;
}

