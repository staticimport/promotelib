#include <iso646.h>
#include <stdlib.h>
#include <string.h>

#include "singly_linked_list.h"

#define PMT_SLL_MOD_COUNT(sll) ((sll)->sentinel_tail.item.szt)
#define PMT_SLL_SIZE(sll) ((sll)->sentinel_head.item.szt)

/* Init / Uninit */
bool
pmt_sllist_init(pmt_sllist_t *const restrict list,
                pmt_allocator_t *const restrict allocator)
{
    assert(list);
    list->sentinel_head.next = &(list->sentinel_tail);
    list->sentinel_tail.next = &(list->sentinel_head);
    PMT_SLL_SIZE(list) = 0;
    PMT_SLL_MOD_COUNT(list) = 0;
    if( allocator ) {
        pmt_allocator_init_copy(&(list->allocator), allocator);
    } else {
        pmt_allocator_init_standard(&(list->allocator));
    }
    return true;
}

bool
pmt_sllist_uninit(pmt_sllist_t *const restrict list)
{
    while( PMT_SLL_SIZE(list) ) {
        pmt_sllist_remove_front(list);
    }
    return true;
}

bool
pmt_sllist_insert(pmt_sllist_t *const restrict list,
                  pmt_universal_t const item,
                  size_t const index)
{
    if( index and (index != PMT_SLL_SIZE(list)) ) {
        // Move to position in list
        pmt_sllist_entry_t *next, *previous;
        next = list->sentinel_head.next;
        for(size_t ii = 0; ii != index; ++ii) {
            next = (previous = next)->next;
        }

        // Create new entry
        pmt_sllist_entry_t *const restrict entry =
            pmt_allocator_alloc(&(list->allocator), PMT_SLL_ALLOC_SIZE, false);
        if( entry ) {
            entry->item = item;
            previous->next = entry;
            entry->next = next;
            ++PMT_SLL_SIZE(list);
            ++PMT_SLL_MOD_COUNT(list);
            return true;
        } else {
            return false;
        }
    } else if( index ) {
        return pmt_sllist_add_back(list,item);
    } else {
        return pmt_sllist_add_front(list,item);
    }
}

bool
pmt_sllist_iterator_init(pmt_sllist_t *const restrict list,
                         pmt_sllist_iterator_t *const restrict iterator)
{
    iterator->list = list;
    iterator->entry = &(list->sentinel_head);
    iterator->valid_mod_count = PMT_SLL_MOD_COUNT(list);
    return true;
}

#undef PMT_SLL_MOD_COUNT
#undef PMT_SLL_SIZE

