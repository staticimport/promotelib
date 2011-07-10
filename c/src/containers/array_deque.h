#ifndef PROMOTE_ARRAY_DEQUE_H_
#define PROMOTE_ARRAY_DEQUE_H_

#include <assert.h>
#include <iso646.h>
#include <stddef.h>
#include <stdbool.h>

#include "universal.h"

typedef struct pmt_arraydeque {
    pmt_universal_t* array;
    size_t front;
    size_t size;
    size_t capacity;
    size_t capacity_log2;
    size_t mask;
} pmt_arraydeque_t;

/* Init/Uninit */
void 
pmt_arraydeque_init(pmt_arraydeque_t* const restrict deque,
                   size_t const min_init_capacity);

void 
pmt_arraydeque_uninit(pmt_arraydeque_t* const restrict deque);

/* Properties */
static inline bool
pmt_arraydeque_is_empty(pmt_arraydeque_t const* const restrict deque)
{
    return deque->size == 0;
}

static inline size_t
pmt_arraydeque_size(pmt_arraydeque_t const* const restrict deque)
{
    return deque->size;
}

/* Observation */
static inline pmt_universal_t
pmt_arraydeque_back(pmt_arraydeque_t const* const restrict deque)
{
    assert(deque->size);
    return deque->array[(deque->front + deque->size - 1) & deque->mask];
}

static inline pmt_universal_t
pmt_arraydeque_front(pmt_arraydeque_t const* const restrict deque)
{
    assert(deque->size);
    return deque->array[deque->front & deque->mask];
}

static inline pmt_universal_t
pmt_arraydeque_get(pmt_arraydeque_t const* const restrict deque,
                  size_t const index)
{
    assert(index < deque->size);
    return deque->array[(deque->front + index) & deque->mask];
}

/* Modify */
bool
pmt_arraydeque_ensure_capacity(pmt_arraydeque_t* const restrict deque,
                              size_t const min_capacity);

static inline bool
pmt_arraydeque_add_back(pmt_arraydeque_t* const restrict deque,
                       pmt_universal_t const item)
{
    if( deque->size < deque->capacity or pmt_arraydeque_ensure_capacity(deque, deque->capacity << 1) ) {
        deque->array[(deque->front + (deque->size)++) & deque->mask] = item;
        return true;
    } else {
        return false;
    }
}

static inline bool
pmt_arraydeque_add_front(pmt_arraydeque_t* const restrict deque,
                        pmt_universal_t const item)
{
    if( deque->size < deque->capacity or pmt_arraydeque_ensure_capacity(deque, deque->capacity << 1) ) {
        deque->array[deque->front = ((deque->front - 1) & deque->mask)] = item;
        ++(deque->size);
        return true;
    } else {
        return false;
    }
}


/*static inline pmt_universal_t
pmt_arraydeque_remove_index(pmt_arraydeque_t* const restrict deque,
                           size_t const index)
{
    assert(false);
}*/

static inline void
pmt_arraydeque_set(pmt_arraydeque_t* const restrict deque,
                  size_t const index,
                  pmt_universal_t const item)
{
    assert(index < deque->size);
    deque->array[(deque->front + index) & deque->mask] = item;
}

#endif /* PROMOTE_ARRAY_DEQUE_H_ */

