
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "stopwatch.h"
#include "testing.h"

#ifndef __APPLE__
static uint64_t perf_test_clock_gettime(clockid_t id);
#endif
static uint64_t perf_test_gettimeofday();
static uint64_t perf_test_rdtsc();
static inline uint64_t my_rdtsc();

int main(void)
{
#ifndef __APPLE__
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
#endif
  pro_print_perf_result("gettimeofday", perf_test_gettimeofday());
  pro_print_perf_result("rdtsc", perf_test_rdtsc());
  return 0;
}

#ifndef __APPLE__
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
#endif

static uint64_t perf_test_gettimeofday()
{
  struct timeval tv;
  pro_stopwatch_t stopwatch;
  uint64_t const iters = 1000000;
  pro_stopwatch_init(&stopwatch);
  pro_stopwatch_start(&stopwatch);
  for(uint64_t ii = 0; ii != iters; ++ii) {
    gettimeofday(&tv, 0);
  }
  pro_stopwatch_stop(&stopwatch);
  return pro_stopwatch_elapsed_nanos(&stopwatch) / iters;
}

static uint64_t perf_test_rdtsc()
{
  pro_stopwatch_t stopwatch;
  uint64_t const iters = 1000000;
  uint64_t out = 0, tmp;
  uint64_t diffs = 0;
  pro_stopwatch_init(&stopwatch);
  pro_stopwatch_start(&stopwatch);
  for(uint64_t ii = 0; ii != iters; ++ii) {
    tmp = my_rdtsc();
    if (out != 0) {
      diffs += (tmp - out);
    }
    out = tmp;
    //my_rdtsc();
    //rdtscll(out);
    //rdtsc(out, out);
  }
  pro_stopwatch_stop(&stopwatch);
  printf("Average diff: %f\n", ((double)diffs) / iters);
  return pro_stopwatch_elapsed_nanos(&stopwatch) / iters;
}

static inline uint64_t my_rdtsc()
{
  unsigned a,d;
  //asm("cpuid");
  //asm ("rdtsc" : "=a" (a), "=d" (d));
  asm volatile ("rdtscp" : "=a" (a), "=d" (d));
  return (uint64_t)a | (((uint64_t)d) << 32);
  return 0;
}

