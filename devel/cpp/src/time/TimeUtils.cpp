#include "TimeUtils.hpp"

#include <ctime>
#include <stdint.h>
#include <sys/time.h>

#include "AsciiToNumeric.hpp"

uint64_t promote::getCurrentMillis()
{
    struct timeval time;
    gettimeofday(&time, 0);
    return (time.tv_sec * ((uint64_t)1000)) + (time.tv_usec / 1000);
}

uint64_t 
promote::getTodayMillis(unsigned hour, unsigned minute,
                        unsigned second, unsigned millisecond)
{
    struct timeval time;
    time_t seconds;
    struct tm time_info;

    // Get Now
    gettimeofday(&time, 0);
    seconds = static_cast<time_t>(time.tv_sec);
    localtime_r(&seconds, &time_info);

    // Update TM struct
    time_info.tm_hour = hour;
    time_info.tm_min = minute;
    time_info.tm_sec = second;

    return (1000ULL * static_cast<uint64_t>(mktime(&time_info))) + millisecond;
}

uint64_t promote::getTodayMillis(char const* str, std::size_t size)
{
    std::size_t index(0);
    unsigned hour(0);
    unsigned minute(0);
    unsigned second(0);
    unsigned millisecond(0);

    if( AsciiToNumeric<unsigned>::parse(str, size, hour, &index) ) {
        if( index < size and (str[index++] == ':') and
            AsciiToNumeric<unsigned>::parse(str + index, size - index, minute, &index) ) 
        {
            if( index < size and (str[index++] == ':') and
                AsciiToNumeric<unsigned>::parse(str + index, size - index, second, &index) )
            {
                if( index < size and (str[index++] == ':') ) {
                    AsciiToNumeric<unsigned>::parse(str + index, size - index, millisecond);
                }
            }
        }
    }

    return getTodayMillis(hour, minute, second, millisecond);
}

uint64_t promote::getTimevalAsMicros(const struct timeval& time)
{
    return (time.tv_sec * ((uint64_t)1000000)) + time.tv_usec;
}

double promote::getTimevalAsMillisFloating(const struct timeval& time)
{
    return (time.tv_sec * 1000.0) + (time.tv_usec / 1000.0);
}

uint64_t promote::getTimevalAsMillisIntegral(const struct timeval& time)
{
    return (time.tv_sec * ((uint64_t)1000)) + (time.tv_usec / 1000);
}

void promote::timevalDiff(struct timeval& result, struct timeval& first, struct timeval& second)
{
    if( first.tv_usec >= second.tv_usec ) {
        result.tv_sec = first.tv_sec - second.tv_sec;
        result.tv_usec = first.tv_usec - second.tv_usec;
    } else {
        result.tv_sec = first.tv_sec - second.tv_sec - 1;
        result.tv_usec = 1000000  + first.tv_usec - second.tv_usec;
    }
}

bool promote::timevalDiffSafe(struct timeval& result, struct timeval& first, struct timeval& second)
{
    if( first.tv_sec > second.tv_sec ) {
        timevalDiff(result, first, second);
        return true;
    } else if( first.tv_sec == second.tv_sec and first.tv_usec > second.tv_usec ) {
        result.tv_sec = 0;
        result.tv_usec = first.tv_usec - second.tv_usec;
        return true;
    } else {
        result.tv_sec = 0;
        result.tv_usec = 0;
        return false;
    }
}

void promote::timevalTotal(struct timeval& result, struct timeval& first, struct timeval& second)
{
    result.tv_usec = first.tv_usec + second.tv_usec;
    if( result.tv_usec < 1000000 )
        result.tv_sec = first.tv_sec + second.tv_sec;
    else {
        result.tv_sec = first.tv_sec + second.tv_sec + 1;
        result.tv_usec -= 1000000;
    }
}


