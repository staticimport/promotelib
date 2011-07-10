#ifndef PROMOTE_TIMESTAMP_HPP_
#define PROMOTE_TIMESTAMP_HPP_

#include <cassert>
#include <ctime>

#include "TimeUtils.hpp"

namespace promote
{
    class Timestamp
    {
    public:
        enum DayOfWeek {
            SUNDAY=0, MONDAY, TUESDAY, WEDNESDAY,
            THURSDAY, FRIDAY, SATURDAY
        };

        Timestamp();
        ~Timestamp() { }

        /* Const */
        unsigned dayOfMonth() const     { return _tm.tm_mday; }
        DayOfWeek dayOfWeek() const     { return static_cast<DayOfWeek>(_tm.tm_wday); }
        unsigned dayOfYear() const      { return _tm.tm_yday; }
        unsigned hour() const           { return _tm.tm_hour; }
        bool isDST() const              { return static_cast<bool>(_tm.tm_isdst); }
        bool isLeapYear() const         { return promote::isLeapYear(year()); }
        unsigned microsecond() const    { return _micros; }
        unsigned millisecond() const    { return _millis; }
        unsigned minute() const         { return _tm.tm_min; }
        unsigned month() const          { return _tm.tm_mon+1; }
        unsigned nanosecond() const     { return _nanos; }
        unsigned second() const         { return _tm.tm_sec; }
        char const* timeZone() const    { return _tm.tm_zone; }
        unsigned year() const           { return 1900 + _tm.tm_year; }

        /* Non-Const */
        void dayOfMonth(int const mday)
        {
            _tm.tm_mday = mday;
            mktime(&_tm);
        }
        void hour(int const hour)
        {
            _tm.tm_hour = hour;
            mktime(&_tm);
        }
        void isDST(bool const isDST)
        {
            _tm.tm_isdst = isDST ? 1 : 0;
            mktime(&_tm);
        }
        void microsecond(int const micro)
        {
            if( micro >= 0 and micro < 1000 ) {
                _micros = micro;
            } else {
                _micros = micro % 1000;
                if( _micros < 0 ) {
                    _micros += 1000;
                }
                millisecond(millisecond() + micro / 1000);
            }
        }
        void millisecond(int const millisecond)
        {
            if( millisecond >= 0 and millisecond < 1000 ) {
                _millis = millisecond;
            } else {
                _millis = millisecond % 1000;
                if( _millis < 0 ) {
                    _millis += 1000;
                }
                second(second() + millisecond / 1000);
            }
        }
        void minute(int const minute)
        {
            _tm.tm_min = minute;
            mktime(&_tm);
        }
        void month(int const month)
        {
            assert(month);
            _tm.tm_mon = month > 0 ? month-1 : month;
            mktime(&_tm);
        }
        void nanosecond(int const nanosecond)
        {
            if( nanosecond >= 0 and nanosecond < 1000 ) {
                _nanos = nanosecond;
            } else {
                _nanos = nanosecond % 1000;
                if( _nanos < 0 ) {
                    _nanos += 1000;
                }
                microsecond(microsecond() + nanosecond / 1000);
            }
        }
        void second(int const sec)
        {
            _tm.tm_sec = sec;
            mktime(&_tm);
        }
        void year(int const year)
        {
            _tm.tm_year = year - 1900;
            mktime(&_tm);
        }
        void date(unsigned const month,
                  unsigned const dayOfMonth,
                  unsigned const year)
        {
            _tm.tm_mon = month - 1;
            _tm.tm_mday = dayOfMonth;
            _tm.tm_year = year - 1900;
            mktime(&_tm);
        }
    private:
        struct tm _tm;
        int _millis;
        int _micros;
        int _nanos;
        bool _needsMake;
    };
}

#endif /* PROMOTE_TIMESTAMP_HPP_ */

