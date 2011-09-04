#ifndef PROMOTE_STRING_HPP_
#define PROMOTE_STRING_HPP_

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <stdint.h>

#include "Comparator.hpp"
#include "Hash.hpp"

namespace promote
{
    class String
    {
    public:
#ifdef __LP64__
        typedef uint64_t size_t;
        static const size_t MAX_LENGTH = ((1ULL << 63) - 1);
#else
        typedef uint32_t size_t;
        static const size_t MAX_LENGTH = ((1U << 31) - 1);
#endif
        static const size_t npos = MAX_LENGTH;

        String();
        String(char const* data, bool const copy=true);
        String(char const* data, size_t const length, bool const copy=true);
        String(String const& string);
        virtual ~String();

        const char* c_str() const       { return _data; }
        size_t length() const           { return _length & MAX_LENGTH; }
        hash_t hashcode() const         { return _hashcode; }
        char operator[](std::size_t const index) const
        {
            return _data[index];
        }

        size_t find(char c, size_t pos = npos) const
        {
            size_t lastIndex(std::min<size_t>(length(), pos));
            for(size_t ii = 0; ii <= lastIndex; ++ii) {
                if( _data[ii] == c ) {
                    return ii;
                }
            }
            return npos;
        }

        size_t rfind(char c, size_t pos = npos) const
        {
            for(size_t ii = std::min<size_t>(length(), pos); ii; --ii) {
                if( _data[ii] == c ) {
                    return ii;
                }
            }
            return (_data[0] == c ? 0 : npos);
        }

        void set(const char* data, size_t length, bool copy=true);
        void setDeepCopy();
        void trim();

        String& operator=(String const& o)
        {
            if( this != &o ) {
                set(o.c_str(), o.length());
            }
            return *this;
        }

        bool operator==(const String& o) const
        {
            const size_t len(length());
            return len == o.length() and (memcmp(_data, o.c_str(), len) == 0);
        }

        bool operator!=(String const& o) const
        {
            return !(*this == o);
        }

        Order compareTo(const String& o) const
        {
            const size_t len(length());
            const size_t oLength(o.length());
            if( len != oLength ) {
                if( len < oLength ) {
                    const int overlapComp(memcmp(_data, o.c_str(), len));
                    return overlapComp <= 0 ? ORDER_INCREASING : ORDER_DECREASING;
                } else {
                    const int overlapComp(memcmp(_data, o.c_str(), oLength));
                    return overlapComp < 0 ? ORDER_INCREASING : ORDER_DECREASING;
                }
            } else {
                const int comp(memcmp(_data, o.c_str(), len));
                if( comp < 0 ) {
                    return ORDER_INCREASING;
                } else if( comp ) {
                    return ORDER_DECREASING;
                } else {
                    return ORDER_SAME;
                }
            }
        }

        bool operator<(const String& o) const
        {
            return (compareTo(o) == ORDER_INCREASING);
        }

        bool operator<=(const String& o) const
        {
            return (compareTo(o) != ORDER_DECREASING);
        }

        bool operator>(const String& o) const
        {
            return (compareTo(o) == ORDER_DECREASING);
        }

        bool operator>=(const String& o) const
        {
            return (compareTo(o) != ORDER_INCREASING);
        }

    private:
#ifdef __LP64__
        static const size_t FREE_FLAG = (1ULL << 63);
#else
        static const size_t FREE_FLAG = (1U << 31);
#endif

        const char* _data;
        size_t _length;
        promote::hash_t _hashcode;
    };
}

#endif /* PROMOTE_STRING_HPP_ */

