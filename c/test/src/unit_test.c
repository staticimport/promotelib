
#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "item_test.h"
#include "timeval_test.h"

static bool is_contained(char const* const* const array, int len,
                         char const* const s);

static void unit_test_common();
static void unit_test_time();

int main(int argc, char const* const* argv)
{
  bool const run_all = (argc == 1);
  int const arg_count = argc - 1;
  char const* const* const args = argv + 1;

  // Common
  if (run_all or is_contained(args, arg_count, "common")) {
    printf("\n[[ src/common ]]\n");
    unit_test_common();
  }

  // Time
  if (run_all or is_contained(args, arg_count, "time")) {
    printf("\n[[ src/time ]]\n");
    unit_test_time();
  }

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

static void unit_test_common()
{
  pro_unit_test_item();
}

static void unit_test_time()
{
  pro_unit_test_timeval();
}

