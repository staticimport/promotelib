
#include <assert.h>

#include "treap_set.h"

static inline void rotate_up(pmt_treapset_t* const restrict set,
                             pmt_treapset_entry_t* const restrict entry);

bool 
pmt_treapset_init(pmt_treapset_t *restrict set,
                  pmt_allocator_t *restrict allocator,
                  pmt_comparator_t *restrict comparator)
{
    assert(set);
    set->root = 0;
    set->size = 0;
    pmt_randgen_init(&(set->randgen), 0, 0);
    if( comparator ) {
        pmt_comparator_init_copy(&(set->comparator), comparator);
    } else {
        pmt_comparator_init(&(set->comparator), 0, 0);
    }
    if( allocator ) {
        pmt_allocator_init_copy(&(set->allocator), allocator);
    } else {
        pmt_allocator_init_standard(&(set->allocator));
    }
    return true;
}

void pmt_treapset_uninit(pmt_treapset_t *restrict set)
{
    
}

bool pmt_treapset_insert(pmt_treapset_t *restrict set,
                         pmt_universal_t key)
{
    pmt_treapset_entry_t* entry = set->root;
    pmt_treapset_entry_t* parent = 0;
    bool is_left = false;
    while( entry ) {
        switch(pmt_comparator_compare(&(set->comparator), key, entry->key)) {
            case PMT_ORDER_INCREASING:
                entry = (parent = entry)->left;
                is_left = true;
                break;
            case PMT_ORDER_DECREASING:
                entry = (parent = entry)->right;
                is_left = false;
                break;
            case PMT_ORDER_SAME:
                return true;
        }
    }

    // Alloc entry
    pmt_treapset_entry_t* const restrict new_entry =
        pmt_allocator_alloc(&(set->allocator), sizeof(pmt_treapset_entry_t), false);
    if( new_entry == 0 ) {
        return false;
    }
    new_entry->key = key;
    entry->heap_key = pmt_randgen_generate(&(set->randgen));
    new_entry->parent = parent;
    new_entry->left = new_entry->right = 0;

    if( parent ) {
        if( is_left ) {
            parent->left = new_entry;
        } else {
            parent->right = new_entry;
        }
        rotate_up(set, entry);
    } else {
        set->root = entry;
    }
    ++(set->size);
    return true;
}

static inline void rotate_up(pmt_treapset_t* const restrict set,
                             pmt_treapset_entry_t* const restrict entry)
{
    pmt_rand_t const heap_key = entry->heap_key;
    for(pmt_treapset_entry_t* parent = entry->parent; parent; parent = entry->parent) {
        if( heap_key < parent->heap_key ) {
            if( parent->left == entry ) {
                PROMOTE_TREE_ROTATE_RIGHT(parent, pmt_treapset_entry_t)
            } else {
                PROMOTE_TREE_ROTATE_LEFT(parent, pmt_treapset_entry_t)
            }
        } else {
            break;
        }
    }
    if( entry->parent == 0 ) {
        set->root = entry;
    }
}

