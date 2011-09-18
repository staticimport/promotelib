
#include <cassert>

#include "StopWatch.hpp"
#include "StopWatchTest.hpp"
#include "Testing.hpp"

using namespace promote;
using namespace promote::testing;

static double perfTestStartAndStop();

static std::string unitTestReset();
static std::string unitTestStart();
static std::string unitTestStop();

void promote::testing::perfTestStopWatch()
{
  printPerfResult("StopWatch: start() + stop()", perfTestStartAndStop());
}

void promote::testing::unitTestStopWatch()
{
  printUnitResult("StopWatch: reset()", unitTestReset());
  printUnitResult("StopWatch: start()", unitTestStart());
  printUnitResult("StopWatch: stop()", unitTestStop());
}

static double perfTestStartAndStop()
{
  StopWatch stopwatch;
  StopWatch sw;
  int64_t const iters(1000000);
  stopwatch.start();
  for(int64_t ii = 0; ii != iters; ++ii) {
    sw.start();
    sw.stop();
  }
  stopwatch.stop();
  if (sw.elapsed().nanoseconds() < 0) { // Impossible, prevent compiler over-optimize
    assert(false);
  }
  return stopwatch.elapsed().nanoseconds() / ((double)iters);
}

/*static double perfTestStop()
{
  StopWatch stopwatch;
  StopWatch sws[1000];
  int64_t const iters(sizeof sws / sizeof(StopWatch));
  for(int64_t ii = 0; ii != iters; ++ii) {
    sws[ii].start();
  }
  stopwatch.start();
  for(int64_t ii = 0; ii != iters; ++ii) {
    sws[ii].stop();
  }
  stopwatch.stop();
  return stopwatch.elapsed().nanoseconds() / ((double)iters);
}*/

static std::string unitTestReset()
{
  StopWatch sw;
  sw.reset();
  if (sw.elapsed().nanoseconds() != 0 or sw.running()) {
    return "Reset() on new StopWatch failed.";
  }
  sw.start();
  sw.reset();
  if (sw.elapsed().nanoseconds() != 0 or sw.running()) {
    return "Reset() on started StopWatch failed.";
  }
  sw.start();
  sw.stop();
  sw.reset();
  if (sw.elapsed().nanoseconds() != 0 or sw.running()) {
    return "Reset() on stopped StopWatch failed.";
  }
  return "";
}

static std::string unitTestStart()
{
  StopWatch sw;
  try {
    sw.start();
  } catch(...) {
    return "Start() on new StopWatch threw an exception.";
  }
  if (!sw.running()) {
    return "Running() is false after start().";
  }
  try {
    sw.start();
    return "Repeated Start() should have thrown an exception.";
  } catch(...) {
  }
  if (!sw.running()) {
    return "Second start() affected running().";
  }
  return "";
}

static std::string unitTestStop()
{
  StopWatch sw;
  try {
    sw.stop();
    return "Stop() on new StopWatch should have thrown an exception.";
  } catch (...) { }
  if (sw.running()) {
    return "Running() returned true after Stop() on new StopWatch.";
  }
  sw.start();
  try {
    sw.stop();
  } catch (...) {
    return "Stop() on started StopWatch threw an exception.";
  }
  if (sw.running()) {
    return "Running returned true after Stop() on started StopWatch.";
  }
  try {
    sw.stop();
    return "Stop() on stopped StopWatch should have thrown an exception.";
  } catch (...) { }
  if (sw.running()) {
    return "Running() returned true after Stop() on stopped StopWatch.";
  }
  return "";
}

