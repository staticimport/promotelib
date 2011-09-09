
#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>

#include "stopwatch.h"
#include "testing.h"
#include "vector.h"
#include "vector_test.h"

static uint64_t perf_test_index_of(size_t const count);
static uint64_t perf_test_last_index_of(size_t const count);
static uint64_t perf_test_add_back(size_t const count,
                                   bool large_init_capacity);

static char const* unit_test_init();
static char const* unit_test_at_index();
static char const* unit_test_back();
static char const* unit_test_front();
static char const* unit_test_index_of();
static char const* unit_test_is_empty();
static char const* unit_test_last_index_of();
static char const* unit_test_size();
static char const* unit_test_add_at_index();
static char const* unit_test_add_back();
static char const* unit_test_ensure_capacity();
static char const* unit_test_remove_at_index();
static char const* unit_test_remove_back();
static char const* unit_test_remove();
static char const* unit_test_remove_last();
static char const* unit_test_iterator();

void pro_unit_test_vector()
{
  pro_print_unit_result("VECTOR: init", unit_test_init());
  pro_print_unit_result("VECTOR: at_index", unit_test_at_index());
  pro_print_unit_result("VECTOR: back", unit_test_back());
  pro_print_unit_result("VECTOR: front", unit_test_front());
  pro_print_unit_result("VECTOR: index_of", unit_test_index_of());
  pro_print_unit_result("VECTOR: is_empty", unit_test_is_empty());
  pro_print_unit_result("VECTOR: last_index_of", unit_test_last_index_of());
  pro_print_unit_result("VECTOR: size", unit_test_size());
  pro_print_unit_result("VECTOR: add_at_index", unit_test_add_at_index());
  pro_print_unit_result("VECTOR: add_back", unit_test_add_back());
  pro_print_unit_result("VECTOR: ensure_capacity", unit_test_ensure_capacity());
  pro_print_unit_result("VECTOR: remove_at_index", unit_test_remove_at_index());
  pro_print_unit_result("VECTOR: remove_back", unit_test_remove_back());
  pro_print_unit_result("VECTOR: remove", unit_test_remove());
  pro_print_unit_result("VECTOR: remove_last", unit_test_remove_last());
  pro_print_unit_result("VECTOR: iterator", unit_test_iterator());
}

void pro_perf_test_vector()
{
  for(uint64_t ii = 10; ii <= 10000; ii *= 10) {
    pro_print_perf_iters_result("VECTOR: add_back (init small)", ii,
                                perf_test_add_back(ii, false));
  }
  for(uint64_t ii = 10; ii <= 10000; ii *= 10) {
    pro_print_perf_iters_result("VECTOR: add_back (init large)", ii,
                                perf_test_add_back(ii, true));
  }
  for(uint64_t ii = 10; ii <= 10000; ii *= 10) {
    pro_print_perf_iters_result("VECTOR: index_of", ii,
                                perf_test_index_of(ii));
  }
  for(uint64_t ii = 10; ii <= 10000; ii *= 10) {
    pro_print_perf_iters_result("VECTOR: last_index_of", ii,
                                perf_test_last_index_of(ii));
  }
}

static uint64_t perf_test_index_of(size_t const count)
{
  return 0;
}

static uint64_t perf_test_last_index_of(size_t const count)
{
  return 0;
}

static uint64_t perf_test_add_back(size_t const count,
                                   bool large_init_capacity)
{
  pro_stopwatch_t timer;
  pro_item_t item;
  uint64_t const iters = 100000 / count;
  pro_stopwatch_init(&timer);
  pro_vector_t* vectors = malloc(iters * sizeof(pro_vector_t));
  for(size_t ii = 0; ii != iters; ++ii) {
    pro_vector_init(&(vectors[ii]), large_init_capacity ? count : 1);
  }
  pro_stopwatch_start(&timer);
  for(size_t ii = 0; ii != iters; ++ii) {
    for(size_t jj = count; jj; --jj) {
      pro_vector_add_back(&(vectors[ii]), item);
    }
  }
  pro_stopwatch_stop(&timer);
  for(size_t ii = 0; ii != iters; ++ii) {
    pro_vector_uninit(&(vectors[ii]));
  }
  free(vectors);
  return pro_stopwatch_elapsed_nanos(&timer) / iters;
}

static char const* unit_test_init()
{
  for(size_t capacity = 1; capacity != 100000; capacity *= 10) {
    pro_vector_t vector;
    pro_vector_init(&vector, capacity);
    if( vector.array == 0 ) {
      return "Null array after init.";
    } else if( vector.capacity < capacity ) {
      return "Vector capacity less than requested capacity after init.";
    }
    pro_vector_uninit(&vector);
  }
  return 0;
}

static char const* unit_test_at_index()
{
  for(size_t count = 0; count != 100; ++count) {
    pro_vector_t vector;
    pro_vector_init(&vector, 1);
    for(size_t ii = 0; ii != count; ++ii) {
      pro_vector_add_back(&vector, pro_item_sizet(ii));
    }
    for(size_t ii = 0; ii != count; ++ii) {
      pro_item_t item = pro_vector_at_index(&vector, ii);
      if( item.szt != ii ) {
        return "Incorrect value returned by at_index()";
      }
    }
    pro_vector_uninit(&vector);
  }
  return 0;
}

static char const* unit_test_back()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
    if( pro_vector_back(&vector).szt != ii ) {
      return "Incorrect value returned by back()";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_front()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
    if( pro_vector_front(&vector).szt != 0 ) {
      return "Incorrect value returned by front()";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_index_of()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, 
      pro_item_sizet(ii < 50 ? ii : 100-ii));
  }
  for(size_t ii = 0; ii != 50; ++ii) {
    if( pro_vector_index_of(&vector, pro_item_sizet(ii)) != ii ) {
      return "Incorrect value returned by index_of()";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_is_empty()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  if( !pro_vector_is_empty(&vector) ) {
    return "is_empty() incorrectly returned false for an empty vector.";
  }
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
    if( pro_vector_is_empty(&vector) ) {
      return "is_empty() incorrectly returned true for an empty vector.";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_last_index_of()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii <= 100; ++ii) {
    pro_vector_add_back(&vector, 
      pro_item_sizet(ii < 50 ? ii : 100-ii));
  }
  for(size_t ii = 0; ii != 50; ++ii) {
    if( pro_vector_last_index_of(&vector, pro_item_sizet(ii)) != 100-ii ) {
      return "Incorrect value returned by last_index_of()";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_size()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  if( pro_vector_size(&vector) != 0 ) {
    return "size() incorrectly returned non-zero for empty vector.";
  }
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
    if( pro_vector_size(&vector) != ii+1 ) {
      return "Incorrect value returned by size()";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_add_at_index()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
  }
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_at_index(&vector, pro_item_sizet(ii), ii*2);
    if( pro_vector_size(&vector) != 101 + ii ) {
      return "Size not updated after add_at_index()";
    }
  }
  for(size_t ii = 0; ii != 200; ++ii) {
    if( pro_vector_at_index(&vector, ii).szt != ii/2 ) {
      return "Incorrect value in vector after add_at_index()";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_add_back()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
    for(size_t jj = 0; jj <= ii; ++jj) {
      if( pro_vector_at_index(&vector, jj).szt != jj ) {
        return "Incorrect value after add_back()";
      }
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_ensure_capacity()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 1; ii != 100; ++ii) {
    pro_vector_ensure_capacity(&vector, ii);
    if( vector.capacity < ii ) {
      return "ensure_capacity() didn't work!";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_remove_at_index()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
  }
  pro_vector_remove_at_index(&vector, 99);
  pro_vector_remove_at_index(&vector, 50);
  pro_vector_remove_at_index(&vector, 1);
  pro_vector_remove_at_index(&vector, 0);
  if( pro_vector_size(&vector) != 96 ) {
    return "Incorrect size after remove_at_index()";
  }
  size_t expected = 0;
  for(size_t ii = 0; ii != 96; ++ii) {
    while( expected == 0 or expected == 1 or expected == 50 or expected == 99 ) {
      ++expected;
    }
    if( pro_vector_at_index(&vector, ii).szt != expected ) {
      return "Incorrect value in vector after remove_at_index()";
    }
    ++expected;
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_remove_back()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
  }
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_remove_back(&vector);
    if( pro_vector_size(&vector) != 99-ii ) {
      return "Incorrect size after remove_back()";
    }
    for(size_t jj = 0; jj != 99-ii; ++jj) {
      if( pro_vector_at_index(&vector, jj).szt != jj ) {
        return "Incorrect values in vector after remove_back()";
      }
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_remove()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
  }
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
  }
  if( !pro_vector_remove(&vector, pro_item_sizet(0)) or
      !pro_vector_remove(&vector, pro_item_sizet(2)) or
      !pro_vector_remove(&vector, pro_item_sizet(50)) or
      !pro_vector_remove(&vector, pro_item_sizet(99)) )
  {
    return "Failed to remove()";
  } else if( pro_vector_size(&vector) != 196 ) {
    return "Failed to update size after remove()";
  }
  if( pro_vector_remove(&vector, pro_item_sizet(100)) ) {
    return "Failed to reject remove() of non-existant entry.";
  }
  size_t expected = 0;
  for(size_t ii = 0; ii != 96; ++ii) {
    while( expected == 0 or expected == 2 or expected == 50 or expected == 99 ) {
      ++expected;
    }
    if( pro_vector_at_index(&vector, ii).szt != expected ) {
      return "Incorrect values in vector after remove()";
    }
    ++expected;
  }
  for(size_t ii = 96; ii != 196; ++ii) {
    if( pro_vector_at_index(&vector, ii).szt != ii-96 ) {
      return "Incorrect later values in vector after remove()";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_remove_last()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
  }
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
  }
  if( !pro_vector_remove_last(&vector, pro_item_sizet(0)) or
      !pro_vector_remove_last(&vector, pro_item_sizet(2)) or
      !pro_vector_remove_last(&vector, pro_item_sizet(50)) or
      !pro_vector_remove_last(&vector, pro_item_sizet(99)) )
  {
    return "Failed to remove_last()";
  } else if( pro_vector_size(&vector) != 196 ) {
    return "Failed to update size after remove_last()";
  }
  if( pro_vector_remove_last(&vector, pro_item_sizet(100)) ) {
    return "Failed to reject remove_last() of non-existant entry.";
  }
  size_t expected = 0;
  for(size_t ii = 100; ii != 196; ++ii) {
    while( expected == 0 or expected == 2 or expected == 50 or expected == 99 ) {
      ++expected;
    }
    if( pro_vector_at_index(&vector, ii).szt != expected ) {
      return "Incorrect values in vector after remove_last()";
    }
    ++expected;
  }
  for(size_t ii = 0; ii != 100; ++ii) {
    if( pro_vector_at_index(&vector, ii).szt != ii ) {
      return "Incorrect earlier values in vector after remove_last()";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

static char const* unit_test_iterator()
{
  pro_vector_t vector;
  pro_vector_init(&vector, 1);
  for(size_t ii = 0; ii != 100; ++ii) {
    pro_vector_add_back(&vector, pro_item_sizet(ii));
  }
  pro_vector_iterator_t iter;
  pro_vector_iterator_init(&vector, &iter);
  size_t iterated_count = 0;
  while( pro_vector_iterator_has_next(&iter) ) {
    if( iterated_count > 100 ) {
      return "Iterated beyond end of vector.";
    } else if( pro_vector_iterator_next(&iter).szt != iterated_count++ ) {
      return "Incorrect value returned by iterator.";
    }
  }
  pro_vector_uninit(&vector);
  return 0;
}

