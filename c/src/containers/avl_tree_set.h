#ifndef PROMOTE_AVL_TREE_SET_H_
#define PROMOTE_AVL_TREE_SET_H_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "allocator.h"
#include "comparator.h"
#include "stack.h"
#include "tree_utils.h"
#include "universal.h"

typedef struct pmt_avlset_entry {
    struct pmt_avlset_entry *parent, *left, *right;
    pmt_universal_t key;
    unsigned height;
} pmt_avlset_entry_t;

typedef struct pmt_avlset {
    pmt_avlset_entry_t *root;
    pmt_allocator_t allocator;
    pmt_comparator_t comparator;
    pmt_stack_t stack;
    size_t mod_count;
    size_t size;
} pmt_avlset_t;

#define PMT_AVLSET_ALLOC_SIZE (sizeof(pmt_avlset_entry_t))
static inline size_t 
pmt_avlset_alloc_size()
{
    return PMT_AVLSET_ALLOC_SIZE;
}


/* Init / Uninit */
bool 
pmt_avlset_init(pmt_avlset_t *const restrict set,
                pmt_allocator_t *const restrict allocator,
                pmt_comparator_t *const restrict comparator);

bool 
pmt_avlset_uninit(pmt_avlset_t *const restrict set);

/* Observe */
static inline bool
pmt_avlset_contains(pmt_avlset_t const *const restrict set,
                    pmt_universal_t const item)
{
    bool result;
    PROMOTE_TREE_CONTAINS(set->root, pmt_avlset_entry_t, item,
                          &(set->comparator), &result)
    return result;
}

static inline pmt_universal_t
pmt_avlset_first(pmt_avlset_t const *const restrict set)
{
    pmt_universal_t result;
    PROMOTE_TREE_FIRST_KEY(set->root, pmt_avlset_entry_t, &result)
    return result;
}

static inline unsigned
pmt_avlset_height(pmt_avlset_t const *const restrict set)
{
    return set->root ? set->root->height : 0;
}

static inline bool
pmt_avlset_is_empty(pmt_avlset_t const *const restrict set)
{
    return set->size == 0;
}

static inline pmt_universal_t
pmt_avlset_last(pmt_avlset_t const *const restrict set)
{
    pmt_universal_t result;
    PROMOTE_TREE_LAST_KEY(set->root, pmt_avlset_entry_t, &result)
    return result;
}

static inline size_t
pmt_avlset_mod_count(pmt_avlset_t const *const restrict set)
{
    return set->mod_count;
}

static inline size_t
pmt_avlset_size(pmt_avlset_t const *const restrict set)
{
    return set->size;
}

/* Modify */
void
pmt_avlset_insert(pmt_avlset_t *const restrict set,
                  pmt_universal_t const item);

/**
 * Iterator
 **/

#endif /* PROMOTE_AVL_TREE_SET_H_ */

