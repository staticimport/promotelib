
#include <iso646.h>
#include <stdlib.h>

#include "hash_functions.h"
#include "hash_functions_test.h"
#include "stopwatch.h"
#include "testing.h"

/**
 * Performance Tests
 **/
static uint64_t perf_test_sax_item();
static uint64_t perf_test_sax_string(unsigned const len);

void pro_perf_test_hash_functions()
{
  pro_print_perf_result("HASH FUNCTION: SAX item", perf_test_sax_item());
  for(unsigned len = 1; len <= 10000; len *= 10) {
    char buf[128];
    sprintf(buf, "HASH FUNCTION: SAX string (length=%u)", len);
    pro_print_perf_result(buf, perf_test_sax_string(len));
  }
}

static uint64_t perf_test_sax_item()
{
  pro_stopwatch_t timer;
  uint64_t const iters = 100000;
  pro_stopwatch_init(&timer);
  pro_stopwatch_start(&timer);
  for(uint64_t ii = 0; ii != iters; ++ii) {
    pro_hash_sax_item(0, pro_item_uint64(ii));
  }
  pro_stopwatch_stop(&timer);
  return pro_stopwatch_elapsed_nanos(&timer) / iters;
}

static uint64_t perf_test_sax_string(unsigned const len)
{
  pro_stopwatch_t timer;
  char* const s = malloc(len+1);
  for(unsigned ii = 0; ii != len; ++ii) {
    s[ii] = 'a' + (ii % 26);
  }
  s[len] = 0;
  pro_item_t item = pro_item_constptr(s);
  uint64_t const iters = 100000;
  pro_stopwatch_init(&timer);
  pro_stopwatch_start(&timer);
  for(uint64_t ii = 0; ii != iters; ++ii) {
    pro_hash_sax_string(0, item);
  }
  pro_stopwatch_stop(&timer);
  return pro_stopwatch_elapsed_nanos(&timer) / iters;
}

/**
 * Unit Tests
 **/
static char const* unit_test_sax_item();
static char const* unit_test_sax_string();

void pro_unit_test_hash_functions()
{
  pro_print_unit_result("HASH FUNCTION: SAX item", unit_test_sax_item());
  pro_print_unit_result("HASH FUNCTION: SAX string", unit_test_sax_string());
}

static char const* unit_test_sax_item()
{
  pro_hash_t prev;
  for(int ii = 0; ii != 1000; ++ii) {
    pro_item_t item = pro_item_int(ii);
    pro_hash_t hash = pro_hash_sax_item(0,item);
    if (ii and hash == prev) {
      return "Consecutive ints produced the same hash value!";
    }
    pro_item_t item2 = pro_item_int(ii);
    if (hash != pro_hash_sax_item(0,item2)) {
      return "Different items with the same value produced different hash values!";
    }
    prev = hash;
  }
  return "";
}

static char const* unit_test_sax_string()
{
  char const* strings[5] = { "", "12345", "helloworld", "sup yall", "*&^$&^%)(*)" };
  size_t const string_count = sizeof strings / sizeof(char const*);
  pro_hash_t prev;
  for(size_t ii = 0; ii != string_count; ++ii) {
    pro_item_t item = pro_item_constptr(strings[ii]);
    pro_hash_t const hash = pro_hash_sax_string(0,item);
    if (ii and hash == prev) {
      return "Two strings just happened to produce the same hash value?";
    }
    pro_item_t item2 = pro_item_constptr(strings[ii]);
    if (pro_hash_sax_string(0,item2) != hash) {
      return "Different items with the same string produced different hash values.";
    }
    prev = hash;
  }
  return "";
}
