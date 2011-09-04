
#include "stack.h"

bool
pmt_stack_init(pmt_stack_t *const restrict stack,
               size_t const init_capacity)
{
    assert(stack);
    assert(init_capacity);
    stack->array = malloc(sizeof(pmt_universal_t) * init_capacity);
    stack->size = 0;
    stack->capacity = init_capacity;
    stack->mod_count = 0;
    return true;
}

bool
pmt_stack_uninit(pmt_stack_t *const restrict stack)
{
    assert(stack);
    free(stack->array);
    stack->array = 0;
    return true;
}

