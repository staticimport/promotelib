
#include <iso646.h>
#include <string.h>
#include <sys/time.h>

#include "testing.h"
#include "timeval.h"
#include "timeval_test.h"

static char const* test_now_monotonic();
static char const* test_now_wall();
static char const* test_as_micros();
static char const* test_as_nanos();

void pro_perf_test_timeval()
{
}

void pro_unit_test_timeval()
{
  pro_print_unit_result("TIMEVAL: now (monotonic)", test_now_monotonic());
  pro_print_unit_result("TIMEVAL: now (wall)", test_now_wall());
  pro_print_unit_result("TIMEVAL: as micros", test_as_micros());
  pro_print_unit_result("TIMEVAL: as nanos", test_as_nanos());
}

static char const* test_now_monotonic()
{
  // Assert increasing
  pro_timeval_t prev, now;
  memset(&prev, 0, sizeof(pro_timeval_t));
  for(int ii = 0; ii != 100; ++ii) {
    pro_timeval_now_monotonic(&now);
    if (now.seconds < prev.seconds or 
       (now.seconds == prev.seconds and now.nanoseconds < prev.nanoseconds))
    {
      return "Monotonic timestamp went backwards!";
    }
    memcpy(&prev, &now, sizeof(pro_timeval_t));
  }

  return "";
}

static char const* test_now_wall()
{
  for(int ii = 0; ii != 10; ++ii) {
    struct timeval before, after;
    pro_timeval_t tv;
    gettimeofday(&before, 0);
    asm("cpuid"); // serialize
    pro_timeval_now_wall(&tv);
    asm("cpuid"); // serialize
    gettimeofday(&after, 0);

    if (tv.seconds < before.tv_sec or
        (tv.seconds == before.tv_sec and tv.nanoseconds/1000 < before.tv_usec))
    {
      return "Wall time was less than a previous gettimeofday result.";
    } else if (after.tv_sec < tv.seconds or
        (after.tv_sec == tv.seconds and after.tv_usec < tv.nanoseconds/1000))
    {
      return "Wall time was greater than a later gettimeofday result.";
    }
  }

  return "";
}

static char const* test_as_micros()
{
  for(int32_t sec = -1; sec != 2; ++sec) {
    for(int32_t nano = -1000000000; nano < 1000000001; nano += 10000) {
      pro_timeval_t tv = { sec, nano };
      uint64_t as_micros = pro_timeval_as_micros(&tv);
      uint64_t should_be = sec * ((uint64_t)1000000) + nano / 1000;
      if (as_micros != should_be) {
        return "Incorrect as_micros returned";
      }
    }
  }
  return "";
}

static char const* test_as_nanos()
{
  for(int32_t sec = -1; sec != 2; ++sec) {
    for(int32_t nano = -1000000000; nano < 1000000001; nano += 10000) {
      pro_timeval_t tv = { sec, nano };
      uint64_t as_micros = pro_timeval_as_nanos(&tv);
      uint64_t should_be = sec * ((uint64_t)1000000000) + nano;
      if (as_micros != should_be) {
        return "Incorrect as_nanos returned";
      }
    }
  }
  return "";
}

