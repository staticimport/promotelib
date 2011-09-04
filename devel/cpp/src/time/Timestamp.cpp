
#include <sys/time.h>

#include "Timestamp.hpp"

using namespace promote;

Timestamp::Timestamp()
{
    struct timeval time;
    time_t seconds;

    // Get Now
    gettimeofday(&time, 0);
    seconds = static_cast<time_t>(time.tv_sec);
    localtime_r(&seconds, &_tm);
    _millis = time.tv_usec / 1000;
    _micros = time.tv_usec % 1000;
    _nanos = 0;
}

