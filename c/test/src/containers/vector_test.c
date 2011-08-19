
#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>

#include "stopwatch.h"
#include "testing.h"
#include "vector.h"
#include "vector_test.h"

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

void pmt_unit_test_vector()
{
  pmt_print_unit_result("VECTOR.init()", unit_test_init());
  pmt_print_unit_result("VECTOR.at_index()", unit_test_at_index());
  pmt_print_unit_result("VECTOR.back()", unit_test_back());
  pmt_print_unit_result("VECTOR.front()", unit_test_front());
  pmt_print_unit_result("VECTOR.index_of()", unit_test_index_of());
  pmt_print_unit_result("VECTOR.is_empty()", unit_test_is_empty());
  pmt_print_unit_result("VECTOR.last_index_of()", unit_test_last_index_of());
  pmt_print_unit_result("VECTOR.size()", unit_test_size());
  pmt_print_unit_result("VECTOR.add_at_index()", unit_test_add_at_index());
  pmt_print_unit_result("VECTOR.add_back()", unit_test_add_back());
  pmt_print_unit_result("VECTOR.ensure_capacity()", unit_test_ensure_capacity());
  pmt_print_unit_result("VECTOR.remove_at_index()", unit_test_remove_at_index());
  pmt_print_unit_result("VECTOR.remove_back()", unit_test_remove_back());
  pmt_print_unit_result("VECTOR.remove()", unit_test_remove());
  pmt_print_unit_result("VECTOR.remove_last()", unit_test_remove_last());
}

static char const* unit_test_init()
{
  for(size_t capacity = 1; capacity != 100000; capacity *= 10) {
    pmt_vector_t vector;
    pmt_vector_init(&vector, capacity);
    if( vector.array == 0 ) {
      return "Null array after init.";
    } else if( vector.capacity < capacity ) {
      return "Vector capacity less than requested capacity after init.";
    }
    pmt_vector_uninit(&vector);
  }
  return 0;
}

static char const* unit_test_at_index()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_back()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_front()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_index_of()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_is_empty()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_last_index_of()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_size()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_add_at_index()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_add_back()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_ensure_capacity()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_remove_at_index()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_remove_back()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_remove()
{
  return "NOT IMPLEMENTED";
}

static char const* unit_test_remove_last()
{
  return "NOT IMPLEMENTED";
}

