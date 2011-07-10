
#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>

#include "avl_tree_set.h"
#include "avl_tree_set_test.h"
#include "compare_functions.h"
#include "stopwatch.h"
#include "testing.h"

static double perf_test_decreasing_add(size_t const count);
static double perf_test_increasing_add(size_t const count);
static double perf_test_random_add(size_t const count);
//static double perf_test_decreasing_contains(size_t const count);

//static uint64_t perf_test_increasing_add();
//static uint64_t perf_test_random_add();
//static uint64_t perf_test_decreasing_contains();
//static uint64_t perf_test_increasing_contains();
//static uint64_t perf_test_random_contains();

static char const* unit_test_init();
static char const* unit_test_add();
static char const* unit_test_contains();

static pmt_allocator_t ALLOCATOR;
static bool IS_ALLOCATOR_INIT = false;

static pmt_comparator_t COMPARATOR;
static bool IS_COMPARATOR_INIT = false;

static void setup()
{
    if( !IS_ALLOCATOR_INIT ) {
        pmt_allocator_init_standard(&ALLOCATOR);
        IS_ALLOCATOR_INIT = true;
    }
    if( !IS_COMPARATOR_INIT ) {
        pmt_comparator_init(&COMPARATOR, &pmt_compare_sizet, NULL);
        IS_COMPARATOR_INIT = true;
    }
}

void pmt_perf_test_avl_tree_set()
{
    setup();
    pmt_print_perf_result("AVL_TREE_SET.add() x 10 decreasing",
                          perf_test_decreasing_add(10));
    pmt_print_perf_result("AVL_TREE_SET.add() x 100 decreasing",
                          perf_test_decreasing_add(100));
    pmt_print_perf_result("AVL_TREE_SET.add() x 1000 decreasing",
                          perf_test_decreasing_add(1000));
    pmt_print_perf_result("AVL_TREE_SET.add() x 10000 decreasing",
                          perf_test_decreasing_add(10000));
    pmt_print_perf_result("AVL_TREE_SET.add() x 10 increasing",
                          perf_test_increasing_add(10));
    pmt_print_perf_result("AVL_TREE_SET.add() x 100 increasing",
                          perf_test_increasing_add(100));
    pmt_print_perf_result("AVL_TREE_SET.add() x 1000 increasing",
                          perf_test_increasing_add(1000));
    pmt_print_perf_result("AVL_TREE_SET.add() x 10000 increasing",
                          perf_test_increasing_add(10000));
    pmt_print_perf_result("AVL_TREE_SET.add() x 10 random",
                          perf_test_random_add(10));
    pmt_print_perf_result("AVL_TREE_SET.add() x 100 random",
                          perf_test_random_add(100));
    pmt_print_perf_result("AVL_TREE_SET.add() x 1000 random",
                          perf_test_random_add(1000));
    pmt_print_perf_result("AVL_TREE_SET.add() x 10000 random",
                          perf_test_random_add(10000));
}

void pmt_unit_test_avl_tree_set()
{
    setup();
    pmt_print_unit_result("AVL_TREE_SET.init()", unit_test_init());
    pmt_print_unit_result("AVL_TREE_SET.add()", unit_test_add());
    pmt_print_unit_result("AVL_TREE_SET.contains()", unit_test_contains());
}

static double perf_test_decreasing_add(size_t const count)
{
    pmt_stopwatch_t timer;
    uint64_t const iters = 1000000 / count;
    pmt_stopwatch_init(&timer);
    for(size_t ii = 0; ii != iters; ++ii) {
        pmt_avlset_t set;
        pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR);
        pmt_stopwatch_start(&timer);
        for(size_t jj = count; jj != 0; --jj) {
            pmt_avlset_insert(&set, pmt_universal_sizet(jj));
        }
        pmt_stopwatch_stop(&timer);
        pmt_avlset_uninit(&set);
    }

    return (double)(pmt_stopwatch_elapsed_nanos(&timer)) / iters;
}

static double perf_test_increasing_add(size_t const count)
{
    pmt_stopwatch_t timer;
    uint64_t const iters = 1000000 / count;
    pmt_stopwatch_init(&timer);
    for(size_t ii = 0; ii != iters; ++ii) {
        pmt_avlset_t set;
        pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR);
        pmt_stopwatch_start(&timer);
        for(size_t jj = 0; jj != count; ++jj) {
            pmt_avlset_insert(&set, pmt_universal_sizet(jj));
        }
        pmt_stopwatch_stop(&timer);
        pmt_avlset_uninit(&set);
    }

    return (double)(pmt_stopwatch_elapsed_nanos(&timer)) / iters;
}

static double perf_test_random_add(size_t const count)
{
    pmt_stopwatch_t timer;
    uint64_t const iters = 1000000 / count;
    size_t *const randoms = malloc(count * sizeof(size_t));
    pmt_stopwatch_init(&timer);
    for(size_t ii = 0; ii != iters; ++ii) {
        for(size_t jj = 0; jj != count; ++jj) {
            randoms[jj] = rand();
        }
        pmt_avlset_t set;
        pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR);
        pmt_stopwatch_start(&timer);
        for(size_t jj = 0; jj != count; ++jj) {
            pmt_avlset_insert(&set, pmt_universal_sizet(randoms[jj]));
        }
        pmt_stopwatch_stop(&timer);
        pmt_avlset_uninit(&set);
    }

    return (double)(pmt_stopwatch_elapsed_nanos(&timer)) / iters;
}

static char const* unit_test_init()
{
    pmt_avlset_t set;
    if( pmt_avlset_init(NULL, &ALLOCATOR, &COMPARATOR) ) {
        return "Init failed to reject NULL set.";
    } else if( pmt_avlset_init(&set, NULL, &COMPARATOR) ) {
        return "Init failed to reject NULL allocator.";
    } else if( pmt_avlset_init(&set, &ALLOCATOR, NULL) ) {
        return "Init failed to reject NULL comparator.";
    }

    if( !pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR) ) {
        return "Proper init failed.";
    } else if( pmt_avlset_size(&set) != 0 ) {
        return "Size not initialized propertly.";
    } else if( !pmt_avlset_uninit(&set) ) {
        return "Uninit failed.";
    }

    return NULL;
}

static char const* unit_test_add()
{
    pmt_avlset_t set;

    // Increasing
    pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR);
    for(size_t ii = 1; ii != 1000; ++ii) {
        pmt_avlset_insert(&set, pmt_universal_sizet(ii));
        if( pmt_avlset_size(&set) != ii ) {
            pmt_avlset_uninit(&set);
            return "Size not correct after (increasing) insert.";
        }
        for(size_t jj = 1; jj <= ii; ++jj) {
            pmt_avlset_insert(&set, pmt_universal_sizet(jj));
            if( pmt_avlset_size(&set) != ii ) {
                pmt_avlset_uninit(&set);
                return "Size incorrectly incremented after duplicate (increasing) insert.";
            }
        }
    }
    pmt_avlset_uninit(&set);
    
    // Decreasing
    pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR);
    for(size_t ii = 1000; ii != 0; --ii) {
        pmt_avlset_insert(&set, pmt_universal_sizet(ii));
        if( pmt_avlset_size(&set) != (1001 - ii) ) {
            pmt_avlset_uninit(&set);
            return "Size not correct after (decreasing) insert.";
        }
        for(size_t jj = 1000; jj >= ii; --jj) {
            pmt_avlset_insert(&set, pmt_universal_sizet(jj));
            if( pmt_avlset_size(&set) != (1001 - ii) ) {
                pmt_avlset_uninit(&set);
                return "Size incorrectly incremented after duplicate (decreasing) insert.";
            }
        }
    }
    pmt_avlset_uninit(&set);

    return NULL;
}

static char const* unit_test_contains()
{
    pmt_avlset_t set;
    pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR);
    for(size_t ii = 1; ii != 1000; ++ii) {
        pmt_avlset_insert(&set, pmt_universal_sizet(ii));
        for(size_t jj = 1; jj != 2000; ++jj) {
            bool const found = pmt_avlset_contains(&set, pmt_universal_sizet(jj));
            if( jj <= ii and !found ) {
                return "Inserted element not found after increasing insert.";
            } else if( jj > ii and found ) {
                return "Uninserted element found after increasing insert.";
            }
        }
    }
    pmt_avlset_uninit(&set);
   
    pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR);
    for(size_t ii = 1000; ii != 0; --ii) {
        pmt_avlset_insert(&set, pmt_universal_sizet(ii));
        for(size_t jj = 1; jj != 1000; ++jj) {
            bool const found = pmt_avlset_contains(&set, pmt_universal_sizet(jj));
            if( jj >= ii and !found ) {
                return "Inserted element not found after decreasing insert.";
            } else if( jj < ii and found ) {
                return "Uninserted element found after decreasing insert.";
            }
        }
    }
    pmt_avlset_uninit(&set);
    
    pmt_avlset_init(&set, &ALLOCATOR, &COMPARATOR);
    size_t* rand_values = malloc(1000 * sizeof(size_t));
    for(size_t ii = 0; ii != 1000; ++ii) {
        rand_values[ii] = (size_t)rand();
        pmt_avlset_insert(&set, pmt_universal_sizet(rand_values[ii]));
        for(size_t jj = 0; jj <= ii; ++jj) {
            bool const found = pmt_avlset_contains(&set, pmt_universal_sizet(rand_values[jj]));
            if( !found ) {
                return "Inserted element not found after random insert.";
            }
        }
    }
    pmt_avlset_uninit(&set);

    return NULL;
}

