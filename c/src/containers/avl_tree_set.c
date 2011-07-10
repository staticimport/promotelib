
#include <iso646.h>
#include <stdio.h>

#include "avl_tree_set.h"

bool 
pmt_avlset_init(pmt_avlset_t *const restrict set,
                pmt_allocator_t *const restrict allocator,
                pmt_comparator_t *const restrict comparator)
{
    if( set and allocator and comparator ) {
        set->root = NULL;
        set->mod_count = 0;
        set->size = 0;
        if( pmt_stack_init(&(set->stack), 16) ) {
            pmt_allocator_init_copy(&(set->allocator), allocator);
            pmt_comparator_init_copy(&(set->comparator), comparator);
            return true;
        }
    }
    return false;
}

bool 
pmt_avlset_uninit(pmt_avlset_t *const restrict set)
{
    if( set->root ) {
        pmt_stack_add(&(set->stack), pmt_universal_ptr(set->root));
        do {
            pmt_universal_t top = pmt_stack_pop(&(set->stack));
            pmt_avlset_entry_t* const entry = top.ptr;
            if( entry->left ) {
                pmt_stack_add(&(set->stack), pmt_universal_ptr(entry->left));
            }
            if( entry->right ) {
                pmt_stack_add(&(set->stack), pmt_universal_ptr(entry->right));
            }
            pmt_allocator_release(&(set->allocator), entry);
        } while( !pmt_stack_is_empty(&(set->stack)) );
        set->root = NULL;
    }
    pmt_stack_uninit(&(set->stack));
    set->mod_count = set->size = 0;
    return true;
}

static inline unsigned uint_max(unsigned const u1, unsigned const u2)
{
    return u1 > u2 ? u1 : u2;
}

static inline unsigned height(pmt_avlset_entry_t const* const restrict entry)
{
    return 1 + uint_max(entry->left ? entry->left->height : 0,
                        entry->right ? entry->right->height : 0);
}

void
pmt_avlset_insert(pmt_avlset_t *const restrict set,
                  pmt_universal_t const item)
{
    // Find place in tree
    pmt_avlset_entry_t *parent = NULL, *entry = set->root;
    bool is_left_child = false;
    while( entry ) {
        switch(pmt_comparator_compare(&(set->comparator), item, entry->key)) {
            case PMT_ORDER_INCREASING:
                is_left_child = true;
                entry = (parent = entry)->left;
                break;
            case PMT_ORDER_DECREASING:
                is_left_child = false;
                entry = (parent = entry)->right;
                break;
            case PMT_ORDER_SAME:
                return;
            default:
                assert(false);
        }
    }

    // Create new entry and insert
    entry = pmt_allocator_alloc(&(set->allocator), sizeof(pmt_avlset_entry_t), false);
    entry->parent = parent;
    entry->left = entry->right = NULL;
    entry->key = item;
    entry->height = 1;
    if( parent ) {
        if( is_left_child ) {
            parent->left = entry;
        } else {
            parent->right = entry;
        }
    } else {
        set->root = entry;
    }
    ++(set->mod_count);
    ++(set->size);
    
    // Rotate up to maintain AVL height property
    entry = entry->parent;
    while( entry ) {
        pmt_avlset_entry_t* const parent = entry->parent;
        pmt_avlset_entry_t* const left = entry->left;
        pmt_avlset_entry_t* const right = entry->right;
        unsigned const left_height = left ? left->height : 0;
        unsigned const right_height = right ? right->height : 0;
        pmt_avlset_entry_t* new_top = entry;
        if( left and left_height == right_height + 2 ) {
            if( left->left and left->left->height == left_height - 1 ) {
                // Left-Left Heavy
                new_top = left;
                if( (entry->left = left->right) ) entry->left->parent = entry;
                if( (left->right = entry) ) left->right->parent = left;
            } else {
                // Left-Right Heavy
                new_top = left->right;
                if( (left->right = new_top->left) ) left->right->parent = left;
                if( (new_top->left = left) ) new_top->left->parent = new_top;
                if( (entry->left = new_top->right) ) entry->left->parent = entry;
                if( (new_top->right = entry) ) new_top->right->parent = new_top;
            }
        } else if( right and left_height == right_height - 2) {
            if( right->right and right->right->height == right_height - 1 ) {
                // Right-Right Heavy
                new_top = right;
                if( (entry->right = right->left) ) entry->right->parent = entry;
                if( (right->left = entry) ) right->left->parent = right;
            } else {
                // Right-Left Heavy
                new_top = right->left;
                if( (right->left = new_top->right) ) right->left->parent = right;
                if( (new_top->right = right) ) new_top->right->parent = new_top;
                if( (entry->right = new_top->left) ) entry->right->parent = entry;
                if( (new_top->left = entry) ) new_top->left->parent = new_top;
            }
        }

        // Update parent -> new_top
        if( parent ) {
            if( parent->left == entry ) {
                parent->left = new_top;
            } else {
                parent->right = new_top;
            }
            new_top->parent = parent;
        } else {
            set->root = new_top;
            new_top->parent = NULL;
        }

        // Update heights
        if( new_top->left ) new_top->left->height = height(new_top->left);
        if( new_top->right ) new_top->right->height = height(new_top->right);
        new_top->height = height(new_top);

        // Keep going up if parent height needs adjusting
        if( parent and (parent->height != height(parent))) {
            entry = parent;
        } else {
            break;
        }
    }
}

