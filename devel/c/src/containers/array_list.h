#ifndef PROMOTE_ARRAY_LIST_H_
#define PROMOTE_ARRAY_LIST_H_

#include <stddef.h>
#include <stdbool.h>

typedef int pmt_collection_t; // TODO: real implementation elsewhere

typedef struct pmt_arraylist {
  pmt_universal_t* array;
  size_t capacity;
  size_t size;
  int mod_count;
} pmt_arraylist_t;

typedef struct pmt_arraylist_iterator {
  pmt_arraylist_t* list;
  size_t next_index;
  int expected_mod_count;
} pmt_arraylist_iterator_t;

/* Init / Uninit */
void
pmt_arraylist_init(pmt_arraylist_t* const restrict list,
                   size_t const init_capacity);

void
pmt_arraylist_uninit(pmt_arraylist_t* const restrict list);

static bool
pmt_arraylist_add(pmt_arraylist_t* const restrict list,
                  pmt_universal_t const item);

static void
pmt_arraylist_add_at(pmt_arraylist_t* const restrict list,
                     pmt_universal_t const item,
                     size_t const index);

static bool
pmt_arraylist_add_all(pmt_arraylist_t* const restrict list,
                      pmt_collection_t const* const restrict collection);

static bool
pmt_arraylist_add_all_at(pmt_arraylist_t* const restrict list,
                         pmt_collection_t const* const restrict collection,
                         size_t const index);

static void
pmt_arraylist_clear(pmt_arraylist_t* const restrict list);

static void
pmt_arraylist_clone(pmt_arraylist_t const* const restrict from,
                    pmt_arraylist_t* const restrict to);

static bool
pmt_arraylist_contains(pmt_arraylist_t const* const restrict list,
                       pmt_universal_t const item);

static void
pmt_arraylist_ensure_capacity(pmt_arraylist_t* const restrict list,
                              size_t const min_capacity);

static pmt_universal_t
pmt_arraylist_get(pmt_arraylist_t const* const restrict list,
                  size_t const index);

static size_t
pmt_arraylist_index_of(pmt_arraylist_t const* const restrict list,
                       pmt_universal_t const item);

static bool
pmt_arraylist_is_empty(pmt_arraylist_t const* const restrict list);

static bool 
pmt_arraylist_last_index_of(pmt_arraylist_t const* const restrict list,
                            pmt_universal_t const item);

static bool
pmt_arraylist_remove(pmt_arraylist_t* const restrict list,
                     pmt_universal_t const item);

static bool
pmt_arraylist_remove_at(pmt_arraylist_t* const restrict list,
                        size_t const index);

static size_t
pmt_arraylist_size(pmt_arraylist_t const* const restrict list);



#endif /* PROMOTE_ARRAY_LIST_H_ */

