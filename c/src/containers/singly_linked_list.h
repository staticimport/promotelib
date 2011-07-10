#ifndef PROMOTE_SINGLY_LINKED_LIST_H_
#define PROMOTE_SINGLY_LINKED_LIST_H_

#include <assert.h>
#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "allocator.h"
#include "universal.h"

typedef struct pmt_sllist_entry {
    struct pmt_sllist_entry *next;
    pmt_universal_t item;
} pmt_sllist_entry_t;

typedef struct pmt_sllist {
    pmt_sllist_entry_t sentinel_head;
    pmt_sllist_entry_t sentinel_tail;
    pmt_allocator_t allocator;
} pmt_sllist_t;

#define PMT_SLL_ALLOC_SIZE (sizeof(pmt_sllist_entry_t))
#define PMT_SLL_MOD_COUNT(sll) ((sll)->sentinel_tail.item.szt)
#define PMT_SLL_SIZE(sll) ((sll)->sentinel_head.item.szt)

/* Init / Uninit */
bool pmt_sllist_init(pmt_sllist_t *const restrict list,
                     pmt_allocator_t *const restrict allocator);
bool pmt_sllist_uninit(pmt_sllist_t *const restrict list);


/* Observe */
static inline pmt_universal_t 
pmt_sllist_get(pmt_sllist_t const *const restrict list,
               size_t const index)
{
    pmt_sllist_entry_t const *entry = list->sentinel_head.next;
    for(size_t ii = 0; ii != index; ++ii) {
        entry = entry->next;
    }
    return entry->item;
}

static inline pmt_universal_t 
pmt_sllist_back(pmt_sllist_t const *const restrict list)
{
    return list->sentinel_tail.next->item;
}

static inline pmt_universal_t
pmt_sllist_front(pmt_sllist_t const *const restrict list)
{
    return list->sentinel_head.next->item;
}

static inline bool 
pmt_sllist_is_empty(pmt_sllist_t const *const restrict list)
{
    return PMT_SLL_SIZE(list) == 0;
}

static inline size_t
pmt_sllist_mod_count(pmt_sllist_t const* const restrict list)
{
    return PMT_SLL_MOD_COUNT(list);
}

static inline size_t 
pmt_sllist_size(pmt_sllist_t const *const restrict list)
{
    return PMT_SLL_SIZE(list);
}

/* Modify */
static inline bool
pmt_sllist_add_back(pmt_sllist_t *const restrict list,
                    pmt_universal_t const item)
{
    pmt_sllist_entry_t *const restrict entry =
        pmt_allocator_alloc(&(list->allocator), PMT_SLL_ALLOC_SIZE, false);
    if( entry ) {
        entry->next = &(list->sentinel_tail);
        entry->item = item;
        list->sentinel_tail.next->next = entry;
        list->sentinel_tail.next = entry;
        ++PMT_SLL_SIZE(list);
        ++PMT_SLL_MOD_COUNT(list);
        return true;
    } else {
        return false;
    }
}

static inline 
bool pmt_sllist_add_front(pmt_sllist_t *const restrict list,
                          pmt_universal_t const item)
{
    pmt_sllist_entry_t *const restrict entry =
        pmt_allocator_alloc(&(list->allocator), PMT_SLL_ALLOC_SIZE, false);
    if( entry ) {
        entry->next = list->sentinel_head.next;
        entry->item = item;
        list->sentinel_head.next = entry;
        if( !PMT_SLL_SIZE(list)++ ) {
            list->sentinel_tail.next = entry;
        }
        ++PMT_SLL_MOD_COUNT(list);
        return true;
    } else {
        return false;
    }

}

bool
pmt_sllist_insert(pmt_sllist_t *const restrict list,
                  pmt_universal_t const item,
                  size_t const index);

static inline bool
pmt_sllist_remove_front(pmt_sllist_t *const restrict list)
{
    assert(PMT_SLL_SIZE(list));
    pmt_sllist_entry_t *const restrict to_remove = list->sentinel_head.next;
    list->sentinel_head.next = to_remove->next;
    if( --PMT_SLL_SIZE(list) == 0 ) {
        list->sentinel_tail.next = &(list->sentinel_head);
    }
    ++PMT_SLL_MOD_COUNT(list);
    pmt_allocator_release(&(list->allocator), to_remove);
    return true;
}

static inline bool
pmt_sllist_remove_index(pmt_sllist_t *const restrict list,
                        size_t const index)
{
    assert(PMT_SLL_SIZE(list) > index);
    pmt_sllist_entry_t *previous = &(list->sentinel_head);
    pmt_sllist_entry_t *entry = previous->next;
    for(size_t ii = 0; ii != index; ++ii) {
        entry = (previous = entry)->next;
    }
    previous->next = entry->next;
    if( --PMT_SLL_SIZE(list) == 0 ) {
        list->sentinel_tail.next = &(list->sentinel_head);
    }
    ++PMT_SLL_MOD_COUNT(list);
    pmt_allocator_release(&(list->allocator), entry);
    return true;
}

/**
 * Iterator
 **/
typedef struct pmt_sllist_iterator {
    pmt_sllist_t const* restrict list;
    pmt_sllist_entry_t* entry;
    size_t valid_mod_count;
} pmt_sllist_iterator_t;

bool
pmt_sllist_iterator_init(pmt_sllist_t *const restrict list,
                         pmt_sllist_iterator_t *const restrict iterator);

static inline pmt_universal_t
pmt_sllist_iterator_current(pmt_sllist_iterator_t *const restrict iterator)
{
    return iterator->entry->item;
}

static inline bool
pmt_sllist_iterator_has_next(pmt_sllist_iterator_t const* const restrict iterator)
{
    return (iterator->entry->next->next != iterator->entry);
}

static inline bool
pmt_sllist_iterator_is_valid(pmt_sllist_iterator_t const* const restrict iterator)
{
    return (iterator->valid_mod_count == PMT_SLL_MOD_COUNT(iterator->list));
}

static inline pmt_universal_t
pmt_sllist_iterator_next(pmt_sllist_iterator_t *const restrict iterator)
{
    assert(pmt_sllist_iterator_is_valid(iterator));
    iterator->entry = iterator->entry->next;
    return iterator->entry->item;
}

#undef PMT_SLL_MOD_COUNT
#undef PMT_SLL_SIZE

#endif /* PROMOTE_SINGLY_LINKED_LIST_H_ */

