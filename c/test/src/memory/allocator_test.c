
#include <assert.h>
#include <iso646.h>

#include "allocator.h"
#include "allocator_test.h"
#include "stopwatch.h"
#include "testing.h"

static uint64_t perf_test_allocator(enum pro_allocator_type const type,
                                    size_t const alloc_size, bool const zeroed,
                                    bool const large_init_capacity);
static char const* unit_test_allocator(enum pro_allocator_type const type,
                                       bool const zeroed);

void pro_perf_test_allocator()
{
  for(size_t alloc_size = 1; alloc_size <= 1000; alloc_size *= 10) {
    bool zeroed = false;
    do {
      char buf[128];
      sprintf(buf, "CACHE_LINE_ALLOCATOR: alloc(size=%u, zeroed=%s)",
              (unsigned)alloc_size, zeroed ? "true" : "false");
      pro_print_perf_result(buf, 
        perf_test_allocator(PRO_ALLOCATOR_CACHE_LINE, alloc_size, zeroed, false));
      zeroed ^= 1;
    } while (zeroed);
  }
  for(size_t alloc_size = 1; alloc_size <= 1000; alloc_size *= 10) {
    bool zeroed = false;
    do {
      char buf[128];
      sprintf(buf, "STANDARD_ALLOCATOR: alloc(size=%u, zeroed=%s)",
              (unsigned)alloc_size, zeroed ? "true" : "false");
      pro_print_perf_result(buf, 
        perf_test_allocator(PRO_ALLOCATOR_STANDARD, alloc_size, zeroed, false));
      zeroed ^= 1;
    } while (zeroed);
  }
}

void pro_unit_test_allocator()
{
  pro_print_unit_result("CACHE_LINE_ALLOCATOR: alloc/release",
                        unit_test_allocator(PRO_ALLOCATOR_CACHE_LINE, false));
  pro_print_unit_result("CACHE_LINE_ALLOCATOR: alloc/release (zeroed)",
                        unit_test_allocator(PRO_ALLOCATOR_CACHE_LINE, true));
  pro_print_unit_result("POOL_ALLOCATOR: alloc/release",
                        unit_test_allocator(PRO_ALLOCATOR_POOL, false));
  pro_print_unit_result("POOL_ALLOCATOR: alloc/release (zeroed)",
                        unit_test_allocator(PRO_ALLOCATOR_POOL, true));
  pro_print_unit_result("STANDARD_ALLOCATOR: alloc/release",
                        unit_test_allocator(PRO_ALLOCATOR_STANDARD, false));
  pro_print_unit_result("STANDARD_ALLOCATOR: alloc/release (zeroed)",
                        unit_test_allocator(PRO_ALLOCATOR_STANDARD, true));
}

static uint64_t perf_test_allocator(enum pro_allocator_type const type,
                                    size_t const alloc_size, bool const zeroed,
                                    bool const large_init_capacity)
{
  pro_allocator_t allocator;
  switch (type) {
    case PRO_ALLOCATOR_CACHE_LINE:
      pro_allocator_init_cache_line(&allocator);
      break;
    case PRO_ALLOCATOR_POOL:
      pro_allocator_init_pool(&allocator, alloc_size, 1);
      break;
    case PRO_ALLOCATOR_STANDARD:
      pro_allocator_init_standard(&allocator);
      break;
    default:
      assert(false);
      return 0;
  }
  pro_stopwatch_t timer;
  uint64_t iters = 1000;
  pro_stopwatch_init(&timer);
  void** alloced = malloc(iters * sizeof(void*));
  pro_stopwatch_start(&timer);
  for(size_t ii = 0; ii != iters; ++ii) {
    alloced[ii] = pro_allocator_alloc(&allocator, alloc_size, zeroed);
  }
  pro_stopwatch_stop(&timer);
  for(size_t ii = 0; ii != iters; ++ii) {
    pro_allocator_release(&allocator, alloced[ii]);
  }
  free(alloced);
  pro_allocator_uninit(&allocator);
  return pro_stopwatch_elapsed_nanos(&timer) / iters;
}

static char const* unit_test_allocator(enum pro_allocator_type const type,
                                       bool const zeroed)
{
  pro_allocator_t allocator;
  size_t const unit_size = 100;
  switch (type) {
    case PRO_ALLOCATOR_CACHE_LINE:
      pro_allocator_init_cache_line(&allocator);
      break;
    case PRO_ALLOCATOR_POOL:
      pro_allocator_init_pool(&allocator, unit_size, 1);
      break;
    case PRO_ALLOCATOR_STANDARD:
      pro_allocator_init_standard(&allocator);
      break;
    default:
      return "Unsupported allocator type for testing.";
  }
  char* alloced[120];
  size_t const alloc_count = sizeof alloced / sizeof(char*);
  for(size_t ii = 0; ii != alloc_count; ++ii) {
    size_t const needed_size = ii < unit_size ? ii : unit_size;
    alloced[ii] = pro_allocator_alloc(&allocator, needed_size, zeroed);
    if (0 == alloced[ii]) {
      return "Alloc returned NULL.";
    } else if (zeroed) {
      for(size_t jj = 0; jj != needed_size; ++jj) {
        if (0 != alloced[ii][jj]) {
          return "Zeroed alloc wasn't zeroed.";
        }
      }
    }
    if (PRO_ALLOCATOR_CACHE_LINE == type and (((uint64_t)alloced[ii]) % 64 != 0)) {
      return "Alloced wasn't properly cache aligned";
    }
    for(size_t jj = 0; jj != needed_size; ++jj) {
      alloced[ii][jj] = (char)ii;
    }
    for(size_t jj = 0; jj <= ii; ++jj) {
      size_t const size = jj < unit_size ? jj : unit_size;
      for(size_t kk = 0; kk != size; ++kk) {
        if ((char)jj != alloced[jj][kk]) {
          return "Data set in previously alloced unit was corrupted.";
        }
      }
    }
  }
  for(size_t ii = 0; ii != alloc_count; ++ii) {
    pro_allocator_release(&allocator, alloced[ii]);
    for(size_t jj = ii+1; jj < alloc_count; ++jj) {
      size_t const size = jj < unit_size ? jj : unit_size;
      for(size_t kk = 0; kk != size; ++kk) {
        if ((char)jj != alloced[jj][kk]) {
          return "Data was corrupted after another alloced was released.";
        }
      }
    }
  }
  pro_allocator_uninit(&allocator);
  return "";
}
