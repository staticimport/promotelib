
#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "allocator_test.h"
#include "hash_functions_test.h"
#include "item_test.h"
#include "timeval_test.h"
#include "vector_test.h"

static bool is_contained(char const* const* const array, int len,
                         char const* const s);

static void perf_test_common();
static void perf_test_containers();
static void perf_test_hash();
static void perf_test_memory();
static void perf_test_time();

#define CONDITIONAL_RUN_TEST_SUITE(name) \
  if (run_all or is_contained(args, arg_count, #name)) { \
    printf("\n[[ src/%s ]]\n", #name); \
    perf_test_ ## name (); \
  }

int main(int argc, char const* const* argv)
{
  bool const run_all = (argc == 1);
  int const arg_count = argc - 1;
  char const* const* const args = argv + 1;

  CONDITIONAL_RUN_TEST_SUITE(common)
  CONDITIONAL_RUN_TEST_SUITE(containers)
  CONDITIONAL_RUN_TEST_SUITE(hash)
  CONDITIONAL_RUN_TEST_SUITE(memory)
  CONDITIONAL_RUN_TEST_SUITE(time)

  return 0;
}

static bool is_contained(char const* const* const array, int len,
                         char const* const s)
{
  for(int ii = 0; ii != len; ++ii) {
    if (strcmp(array[ii], s) == 0) {
      return true;
    }
  }
  return false;
}

static void perf_test_common()
{
  pro_perf_test_item();
}

static void perf_test_containers()
{
  pro_perf_test_vector();
}

static void perf_test_hash()
{
  pro_perf_test_hash_functions();
}

static void perf_test_memory()
{
  pro_perf_test_allocator();
}

static void perf_test_time()
{
  pro_perf_test_timeval();
}

