#ifndef PROMOTE_STOP_WATCH_HPP_
#define PROMOTE_STOP_WATCH_HPP_

#include <sys/time.h>

#include "TimeUtils.hpp"

namespace promote
{
    class StopWatch
    {
    public:
        StopWatch()  { reset(); }
        ~StopWatch() { }

        const struct timeval& elapsed() const { return _elapsed; }
        void elapsed(struct timeval& elapsed) const
        {
            elapsed.tv_sec = _elapsed.tv_sec;
            elapsed.tv_usec = _elapsed.tv_usec;
        }
        uint64_t elapsedMicros() const
        {
            struct timeval tv;
            elapsed(tv);
            return getTimevalAsMicros(tv);
        }

        void reset()
        {
            _elapsed.tv_sec = 0;
            _elapsed.tv_usec = 0;
            _started = false;
        }

        bool start()
        {
            if( !_started ) {
	            gettimeofday(&_lastStart, 0);
	            return (_started = true);
            } else
	            return false;
        }

        bool stop()
        {
            if( _started ) {
	            struct timeval now, diff;
	            gettimeofday(&now, 0);
                promote::timevalDiffSafe(diff, now, _lastStart);
	            promote::timevalTotal(_elapsed, _elapsed, diff);
	            _started = false;
                return true;
            } else
                return false;
        }

    private:
        struct timeval _elapsed;
        struct timeval _lastStart;
        bool _started;
    };
}

#endif /* PROMOTE_STOP_WATCH_HPP_ */

