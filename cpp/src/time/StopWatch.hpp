#ifndef PROMOTE_STOPWATCH_HPP
#define PROMOTE_STOPWATCH_HPP

#include "TimeVal.hpp"

namespace promote {
  class StopWatch {
  public:
    StopWatch();

    // Const
    TimeVal elapsed() const;
    inline bool running() const;

    // Non-Const
    void reset();
    void start();
    void stop();
  private:
    TimeVal _elapsed;
    TimeVal _lastStart;
  };
}

/**
 * Inline Implementation
 **/
inline bool
promote::StopWatch::running() const
{
  return _lastStart.nanoseconds() != 0;
}

#endif /* PROMOTE_STOPWATCH_HPP */

