#ifndef PROMOTE_TIME_UTILS_HPP_
#define PROMOTE_TIME_UTILS_HPP_

#include <stdint.h>
#include <string>
#include <sys/time.h>

#include "String.hpp"

namespace promote
{
    uint64_t getCurrentMillis();
    uint64_t getTodayMillis(unsigned hour, unsigned minute,
                            unsigned second, unsigned millisecond);
    uint64_t getTodayMillis(char const* str, std::size_t size);
    inline uint64_t getTodayMillis(promote::String const& string)
    {
        return getTodayMillis(string.c_str(), string.length());
    }
    inline uint64_t getTodayMillis(std::string const& string)
    {
        return getTodayMillis(string.c_str(), string.length());
    }
    uint64_t getTimevalAsMicros(const struct timeval& time);
    double getTimevalAsMillisFloating(const struct timeval& time);
    uint64_t getTimevalAsMillisIntegral(const struct timeval& time);
    void timevalDiff(struct timeval& result, struct timeval& first, struct timeval& second);
    bool timevalDiffSafe(struct timeval& result, struct timeval& first, struct timeval& second);
    void timevalTotal(struct timeval& result, struct timeval& first, struct timeval& second);

    inline bool isLeapYear(unsigned const year)
    {
        if( (year % 400) == 0 ) {
            return true;
        } else if( (year % 100) == 0 ) {
            return false;
        } else if( (year & 3) == 0 ) { // % 4
            return true;
        } else {
            return false;
        }
    }


    class TimeUnit
    {
    public:
        enum Value {
            NANOSECOND,
            MICROSECOND,
            MILLISECOND,
            SECOND,
            MINUTE,
            HOUR,
            DAY
        };

        static const TimeUnit& nanosecond()
        {
            static const TimeUnit instance(NANOSECOND, 1, 1.0/24/60/60/1000000000);
            return instance;
        }
        static const TimeUnit& microsecond()
        {
            static const TimeUnit instance(MICROSECOND, 1000, 1.0/24/60/60/1000000);
            return instance;
        }
        static const TimeUnit& millisecond()
        {
            static const TimeUnit instance(MILLISECOND, 1000000, 1.0/24/60/60/1000);
            return instance;
        }
        static const TimeUnit& second()
        {
            static const TimeUnit instance(SECOND, 1000000000, 1.0/24/60/60);
            return instance;
        }
        static const TimeUnit& minute()
        {
            static const TimeUnit instance(MINUTE, 60ULL*1000000000, 1.0/24/60);
            return instance;
        }
        static const TimeUnit& hour()
        {
            static const TimeUnit instance(HOUR, 60ULL*60*1000000000, 1.0/24);
            return instance;
        }
        static const TimeUnit& day()
        {
            static const TimeUnit instance(DAY, 24ULL*60*60*1000000000, 1.0);
            return instance;
        }

        static double convert(double value, const TimeUnit& from, const TimeUnit& to)
        {
            if( from._value < to._value ) {
                return value / (to._inNanoseconds / from._inNanoseconds);
            } else {
                return value * (to._inNanoseconds / from._inNanoseconds);
            }
        }

        const Value enumValue() const { return _value; }

    private:
        TimeUnit(const Value& value, uint64_t inNanoseconds, double inDays) 
        :   _value(value),
            _inNanoseconds(inNanoseconds),
            _inDays(inDays)
        { }
        ~TimeUnit() { }

        const Value _value;
        uint64_t _inNanoseconds;
        double _inDays;
    };
}

#endif /* PROMOTE_TIME_UTILS_HPP_ */
