#ifndef PROMOTE_STACK_H
#define PROMOTE_STACK_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "universal.h"

typedef struct pmt_stack {
    pmt_universal_t* array;
    size_t size;
    size_t capacity;
    size_t mod_count;
} pmt_stack_t;

// Init/Uninit
bool
pmt_stack_init(pmt_stack_t *const restrict stack,
               size_t const init_capacity);

bool
pmt_stack_uninit(pmt_stack_t *const restrict stack);

// Observe
static inline bool
pmt_stack_is_empty(pmt_stack_t const *const restrict stack)
{
    return stack->size == 0;
}

static inline size_t
pmt_stack_mod_count(pmt_stack_t const *const restrict stack)
{
    return stack->mod_count;
}

static inline size_t
pmt_stack_size(pmt_stack_t const *const restrict stack)
{
    return stack->size;
}

static inline pmt_universal_t
pmt_stack_top(pmt_stack_t const *const restrict stack)
{
    assert(stack->size);
    return stack->array[stack->size-1];
}

// Modify
static inline void
pmt_stack_add(pmt_stack_t *const restrict stack,
              pmt_universal_t const item)
{
    if( stack->size == stack->capacity ) {
        stack->capacity <<= 1;
        stack->array = realloc(stack->array, sizeof(pmt_universal_t) * stack->capacity);
        assert(stack->array);
    }
    ++(stack->mod_count);
    stack->array[stack->size++] = item;
}

static inline pmt_universal_t
pmt_stack_pop(pmt_stack_t *const restrict stack)
{
    assert(stack->size);
    ++(stack->mod_count);
    return stack->array[--(stack->size)];
}

#endif /* PROMOTE_STACK_H */

