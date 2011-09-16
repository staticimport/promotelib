
#ifndef __APPLE__
#include <ctime>
#else
extern "C" {
#include <CoreServices/CoreServices.h>
#include <sys/time.h>
}
#endif

#include "TimeVal.hpp"

using namespace promote;

int64_t const TimeVal::NANOS_PER_MICRO = 1000000;
int64_t const TimeVal::NANOS_PER_MILLI = 1000;
int64_t const TimeVal::NANOS_PER_SECOND = 1000000000;

TimeVal TimeVal::monotonic()
{
#ifndef __APPLE__
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC_RAW, &spec);
  return TimeVal(((int64_t)spec.tv_sec * NANOS_PER_SECOND) + (int64_t)spec.tv_nsec);
#else
  UnsignedWide const uw = AbsoluteToNanoseconds(UpTime());
  return TimeVal((((int64_t)uw.hi)<<32)|(uw.lo));
#endif
}

TimeVal TimeVal::wall()
{
#ifndef __APPLE__
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  return TimeVal(((int64_t)spec.tv_sec * NANOS_PER_SECOND) + (int64_t)spec.tv_nsec);
#else
  struct timeval tv;
  gettimeofday(&tv, 0);
  return TimeVal(((int64_t)tv.tv_sec * NANOS_PER_SECOND) +
    ((int64_t)tv.tv_usec * NANOS_PER_MICRO));
#endif
}

