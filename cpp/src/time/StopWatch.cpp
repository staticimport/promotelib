
#include "Exception.hpp"
#include "StopWatch.hpp"

using namespace promote;

StopWatch::StopWatch()
: _elapsed(0),
  _lastStart(0)
{
}

TimeVal StopWatch::elapsed() const
{
  if (running()) {
    return _elapsed + (TimeVal::monotonic() - _lastStart);
  } else {
    return _elapsed;
  }
}

void StopWatch::reset()
{
  _elapsed = 0;
  _lastStart = 0;
}

void StopWatch::start()
{
  if (!running()) {
    _lastStart = TimeVal::monotonic();
  } else { 
    throw Exception("StopWatch::start() repeated call not allowed.");
  }
}

void StopWatch::stop()
{
  if (running()) {
    _elapsed += (TimeVal::monotonic() - _lastStart);
    _lastStart = 0;
  } else {
    throw Exception("StopWatch::stop() repeated calls not allowed.");
  }
}

