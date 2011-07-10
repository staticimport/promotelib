#ifndef PROMOTE_NEW_FIXED_QUEUE_H_
#define PROMOTE_NEW_FIXED_QUEUE_H_

#include <assert.h>
#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>

#include "universal.h"

typedef struct pmt_fixed_queue {
    pmt_universal_t* restrict array;
    size_t capacity;
    size_t mask;
    size_t read;
    size_t write;
} pmt_fixed_queue_t;

/* Init / Unit */
void pmt_fixed_queue_init(pmt_fixed_queue_t *const restrict queue,
                          size_t const min_capacity);

void pmt_fixed_queue_uninit(pmt_fixed_queue_t *const restrict queue);

/* Const */
static inline bool
pmt_fixed_queue_can_read(pmt_fixed_queue_t const* const restrict queue)
{
    return queue->read != queue->write;
}

static inline bool
pmt_fixed_queue_can_write(pmt_fixed_queue_t const* const restrict queue)
{
    return queue->write != queue->read + queue->capacity;
}

static inline size_t 
pmt_fixed_queue_capacity(pmt_fixed_queue_t const* const restrict queue)
{
    return queue->capacity;
}

static inline bool
pmt_fixed_queue_peek(pmt_fixed_queue_t const* const restrict queue,
                     pmt_universal_t* const value)
{
    if( queue->read != queue->write ) {
        *value = queue->array[queue->read & queue->mask];
        return true;
    } else {
        return false;
    }
}

static inline size_t 
pmt_fixed_queue_size(pmt_fixed_queue_t const* const restrict queue)
{
    return queue->write - queue->read;
}

/* Non-Const */
static inline pmt_universal_t
pmt_fixed_queue_read(pmt_fixed_queue_t* const restrict queue)
{
    assert(pmt_fixed_queue_can_read(queue));
    return queue->array[(queue->read)++ & queue->mask];
}

static inline void
pmt_fixed_queue_write(pmt_fixed_queue_t* const restrict queue,
                      pmt_universal_t const value)
{
    assert(pmt_fixed_queue_can_write(queue));
    queue->array[(queue->write)++ & queue->mask] = value;
}

static inline bool
pmt_fixed_queue_try_read(pmt_fixed_queue_t* const restrict queue,
                         pmt_universal_t* const restrict value)
{
    if( pmt_fixed_queue_can_read(queue) ) {
        *value = queue->array[(queue->read)++ & queue->mask];
        return true;
    } else {
        return false;
    }
}

static inline bool
pmt_fixed_queue_try_write(pmt_fixed_queue_t* const restrict queue,
                          pmt_universal_t const value)
{
    if( pmt_fixed_queue_can_write(queue) ) {
        queue->array[(queue->write)++ & queue->mask] = value;
        return true;
    } else {
        return false;
    }
}

#endif /* PROMOTE_NEW_FIXED_QUEUE_H_ */

