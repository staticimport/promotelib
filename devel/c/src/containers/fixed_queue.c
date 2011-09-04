
#include <stdlib.h>

#include "base2.h"
#include "fixed_queue.h"

void pmt_fixed_queue_init(pmt_fixed_queue_t *const restrict queue,
                          size_t const min_capacity)
{
    assert(queue);
    assert(min_capacity);
    queue->capacity = (size_t)pmt_pow2_above64((uint64_t)min_capacity, true);
    queue->mask = queue->capacity - 1;
    queue->array = malloc(queue->capacity * sizeof(pmt_universal_t));
    queue->read = 0;
    queue->write = 0;
}

void pmt_fixed_queue_uninit(pmt_fixed_queue_t *const restrict queue)
{
    free(queue->array);
}

