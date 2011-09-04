#ifndef PROMOTE_TREAP_SET_H_
#define PROMOTE_TREAP_SET_H_

#include "allocator.h"
#include "comparator.h"
#include "random_generator.h"
#include "tree_utils.h"

typedef struct pmt_treapset_entry {
    pmt_universal_t key;
    pmt_rand_t heap_key;
    struct pmt_treapset_entry* parent;
    struct pmt_treapset_entry* left;
    struct pmt_treapset_entry* right;
} pmt_treapset_entry_t;

typedef struct pmt_treapset {
    pmt_treapset_entry_t *root;
    unsigned size;
    pmt_allocator_t allocator;
    pmt_comparator_t comparator;
    pmt_randgen_t randgen;
} pmt_treapset_t;

/* Init / Uninit */
bool pmt_treapset_init(pmt_treapset_t *restrict set,
                       pmt_allocator_t *restrict allocator,
                       pmt_comparator_t *restrict comparator);
void pmt_treapset_uninit(pmt_treapset_t *restrict set);

/* Property */
static inline void
pmt_treapset_randgen(pmt_treapset_t *restrict set,
                     pmt_randgen_t *restrict randgen)
{
    pmt_randgen_init_copy(&(set->randgen), randgen);
}

/* Observe */
static inline bool
pmt_treapset_contains(pmt_treapset_t *restrict set,
                      pmt_universal_t const key)
{
    bool result;
    PROMOTE_TREE_CONTAINS(set->root, pmt_treapset_entry_t, key,
                          &(set->comparator), &result)
    return result;
}

static inline bool pmt_treapset_is_empty(pmt_treapset_t const *const restrict set)
{
    return (set->size == 0);
}
static inline unsigned pmt_treapset_size(pmt_treapset_t const *const restrict set)
{
    return set->size;
}

/* Modify */
bool pmt_treapset_insert(pmt_treapset_t *restrict set,
                         pmt_universal_t item);
//bool pmt_treapset_remove(pmt_treapset_t *restrict set,
//                         pmt_universal_t* copy);

#endif /* PROMOTE_TREAP_SET_H_ */

