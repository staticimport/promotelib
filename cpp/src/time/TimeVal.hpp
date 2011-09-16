#ifndef PROMOTE_TIMEVAL_HPP
#define PROMOTE_TIMEVAL_HPP

#include <cstdint>
#include <iostream>

namespace promote {
  class TimeVal {
  public:
    // Static methods for getting time representation of "now"
    static TimeVal monotonic();
    static TimeVal wall();
    
    // Constructor
    inline TimeVal(int64_t const nanoseconds);

    // Const
    inline int64_t microseconds() const;
    inline int64_t milliseconds() const;
    inline int64_t nanoseconds() const;
    inline int64_t seconds() const;
    inline int64_t subSecondMicros() const;
    inline int64_t subSecondMillis() const;
    inline int64_t subSecondNanos() const;
    inline TimeVal operator+(TimeVal const& tv) const;
    inline TimeVal operator-(TimeVal const& tv) const;
    inline bool operator==(TimeVal const& tv) const;
    inline bool operator!=(TimeVal const& tv) const;
    inline bool operator<(TimeVal const& tv) const;
    inline bool operator<=(TimeVal const& tv) const;
    inline bool operator>(TimeVal const& tv) const;
    inline bool operator>=(TimeVal const& tv) const;

    // Non-Const
    inline TimeVal const& operator+=(TimeVal const& tv);
    inline TimeVal const& operator-=(TimeVal const& tv);

    // Friends
    friend std::ostream& operator<<(std::ostream& out, TimeVal const& tv);
  private:
    static int64_t const NANOS_PER_MICRO;
    static int64_t const NANOS_PER_MILLI;
    static int64_t const NANOS_PER_SECOND;

    int64_t _nanoseconds;
  };
}

inline std::ostream& operator<<(std::ostream& out, promote::TimeVal const& tv);

/**
 * Inline Implementations
 **/
inline promote::TimeVal::TimeVal(int64_t const nanoseconds)
: _nanoseconds(nanoseconds)
{
}
    
inline int64_t 
promote::TimeVal::microseconds() const
{
  return _nanoseconds / NANOS_PER_MICRO;
}

inline int64_t 
promote::TimeVal::milliseconds() const
{
  return _nanoseconds / NANOS_PER_MILLI;
}
    
inline int64_t 
promote::TimeVal::nanoseconds() const
{
  return _nanoseconds;
}
    
inline int64_t 
promote::TimeVal::seconds() const
{
  return _nanoseconds / NANOS_PER_SECOND;
}

inline int64_t 
promote::TimeVal::subSecondMicros() const
{
  return subSecondNanos() / NANOS_PER_MICRO;
}

inline int64_t 
promote::TimeVal::subSecondMillis() const
{
  return subSecondNanos() / NANOS_PER_MILLI;
}
    
inline int64_t 
promote::TimeVal::subSecondNanos() const
{
  return _nanoseconds - (seconds() * NANOS_PER_SECOND);
}

inline promote::TimeVal 
promote::TimeVal::operator+(promote::TimeVal const& tv) const
{
  return TimeVal(_nanoseconds + tv.nanoseconds());
}

inline promote::TimeVal 
promote::TimeVal::operator-(promote::TimeVal const& tv) const
{
  return TimeVal(_nanoseconds - tv.nanoseconds());
}

inline bool 
promote::TimeVal::operator==(promote::TimeVal const& tv) const
{
  return _nanoseconds == tv.nanoseconds();
}

inline bool 
promote::TimeVal::operator!=(promote::TimeVal const& tv) const
{
  return _nanoseconds != tv.nanoseconds();
}
    
inline bool 
promote::TimeVal::operator<(promote::TimeVal const& tv) const
{
  return _nanoseconds < tv.nanoseconds();
}

inline bool 
promote::TimeVal::operator<=(promote::TimeVal const& tv) const
{
  return _nanoseconds <= tv.nanoseconds();
}

inline bool 
promote::TimeVal::operator>(promote::TimeVal const& tv) const
{
  return _nanoseconds > tv.nanoseconds();
}

inline bool 
promote::TimeVal::operator>=(promote::TimeVal const& tv) const
{
  return _nanoseconds >= tv.nanoseconds();
}

// Non-Const
inline promote::TimeVal const& 
promote::TimeVal::operator+=(promote::TimeVal const& tv)
{
  _nanoseconds += tv.nanoseconds();
  return *this;
}

inline promote::TimeVal const& 
promote::TimeVal::operator-=(promote::TimeVal const& tv)
{
  _nanoseconds -= tv.nanoseconds();
  return *this;
}

inline std::ostream& 
operator<<(std::ostream& out, promote::TimeVal const& tv)
{
  out << tv.nanoseconds();
  return out;
}

#endif /* PROMOTE_TIMEVAL_HPP */

