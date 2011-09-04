#ifndef PROMOTE_NUMERIC_UTILS_HPP_
#define PROMOTE_NUMERIC_UTILS_HPP_

#include "Utilities.hpp"

namespace promote
{
    template <typename C, typename T>
    inline bool binarySearch(typename Passable<C const>::Type container,
                             typename Passable<T const>::Type value,
                             std::size_t begin,
                             std::size_t end)
    {
        std::size_t noOneCares;
        return binarySearch<C,T>(container,value,begin,end,noOneCares);
    }

    template <typename C, typename T>
    inline bool binarySearch(typename Passable<C const>::Type container,
                             typename Passable<T const>::Type value,
                             std::size_t begin,
                             std::size_t end,
                             std::size_t& nextIndex)
    {
        std::size_t mid;
        while( begin < end ) {
            mid = begin + ((end - begin) >> 1);
            if( container[mid] < value ) {
                begin = mid + 1;
            } else {
                end = mid;
            }
        }
        nextIndex = begin;
        return ((begin < end) and (container[begin] == value));
    }

    template <typename T>
    inline T gcd(T a, T b)
    {
        while( b ) {
            T const oldB(b);
            b = a - (b * (a / b));
            a = oldB;
        }
        return a;
    }

    template <typename T>
    inline T uintPow(T const x, unsigned const pow)
    {
        if( (pow & 1) ) {
            return x * uintPow<T>(x,pow-1);
        } else if( pow ) {
            T const halfPow(uintPow<T>(x,pow>>1));
            return halfPow * halfPow;
        } else {
            return 1;
        }
    }

    template <typename T> inline bool isIntegral()              { return false; }
    template <> inline bool isIntegral<char>()                  { return true; }
    template <> inline bool isIntegral<int>()                   { return true; }
    template <> inline bool isIntegral<long>()                  { return true; }
    template <> inline bool isIntegral<long long>()             { return true; }
    template <> inline bool isIntegral<short>()                 { return true; }
    template <> inline bool isIntegral<unsigned>()              { return true; }
    template <> inline bool isIntegral<unsigned char>()         { return true; }
    template <> inline bool isIntegral<unsigned short>()        { return true; }
    template <> inline bool isIntegral<unsigned long>()         { return true; }
    template <> inline bool isIntegral<unsigned long long>()    { return true; }

    template <typename T>
    struct Primitive
    {
        static const bool IsIntegral = false;
        static const bool IsFloating = true;
    };

    template <>
    struct Primitive<char>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<int>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<long>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<long long>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<short>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<unsigned>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<unsigned char>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<unsigned short>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<unsigned long>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };

    template <>
    struct Primitive<unsigned long long>
    {
        static const bool IsIntegral = true;
        static const bool IsFloating = false;
    };
}

#endif /* PROMOTE_NUMERIC_UTILS_HPP_ */

