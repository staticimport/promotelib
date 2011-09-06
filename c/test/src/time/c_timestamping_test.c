
#include <stdint.h>
#include <time.h>

#include "stopwatch.h"
#include "testing.h"

static uint64_t perf_test_clock_gettime(clockid_t id);
//static uint64_t perf_test_rdtsc();
//static uint64_t rdtsc();

int main(void)
{
  pro_print_perf_result("clock_gettime(CLOCK_REALTIME)",
                        perf_test_clock_gettime(CLOCK_REALTIME));
  pro_print_perf_result("clock_gettime(CLOCK_MONOTONIC)",
                        perf_test_clock_gettime(CLOCK_MONOTONIC));
  pro_print_perf_result("clock_gettime(CLOCK_MONOTONIC_COARSE)",
                        perf_test_clock_gettime(CLOCK_MONOTONIC_COARSE));
  pro_print_perf_result("clock_gettime(CLOCK_MONOTONIC_RAW)",
                        perf_test_clock_gettime(CLOCK_MONOTONIC_RAW));
  pro_print_perf_result("clock_gettime(CLOCK_PROCESS_CPUTIME_ID)",
                        perf_test_clock_gettime(CLOCK_PROCESS_CPUTIME_ID));
  pro_print_perf_result("clock_gettime(CLOCK_THREAD_CPUTIME_ID)",
                        perf_test_clock_gettime(CLOCK_THREAD_CPUTIME_ID));
  //pro_print_perf_result("rdtsc", perf_test_rdtsc());
  return 0;
}

static uint64_t perf_test_clock_gettime(clockid_t id)
{
  struct timespec ts;
  pro_stopwatch_t stopwatch;
  uint64_t const iters = 1000000;
  pro_stopwatch_init(&stopwatch);
  pro_stopwatch_start(&stopwatch);
  for(uint64_t ii = 0; ii != iters; ++ii) {
    clock_gettime(id, &ts);
  }
  pro_stopwatch_stop(&stopwatch);
  return pro_stopwatch_elapsed_nanos(&stopwatch) / iters;
}

/*static uint64_t perf_test_rdtsc()
{
  pro_stopwatch_t stopwatch;
  uint64_t const iters = 1000000;
  pro_stopwatch_init(&stopwatch);
  pro_stopwatch_start(&stopwatch);
  for(uint64_t ii = 0; ii != iters; ++ii) {
    rdtsc();
  }
  pro_stopwatch_stop(&stopwatch);
  return pro_stopwatch_elapsed_nanos(&stopwatch) / iters;
}*/

//static uint64_t rdtsc()
//{
//  unsigned a,d;
//  //asm("cpuid");
//  asm volatile("rdtsc" : "=a" (a), "=d" (d));
//  return (uint64_t)a | (((uint64_t)d) << 32);
//}

