#ifndef PROMOTE_POWER_HPP_
#define PROMOTE_POWER_HPP_

#include <stdint.h>

namespace promote
{
    const uint64_t POWER_10s[] = {
        1ULL,                  10ULL,                 100ULL,                1000ULL,
        10000ULL,              100000ULL,             1000000ULL,            10000000ULL,
        100000000ULL,          1000000000ULL,         10000000000ULL,        100000000000ULL,
        1000000000000ULL,      10000000000000ULL,     100000000000000ULL,    1000000000000000ULL,
        10000000000000000ULL,  100000000000000000ULL, 1000000000000000000ULL
    };
    const unsigned POWER_10s_LENGTH = 19;

    uint64_t leastPower2Above64(uint64_t value, bool inclusive);

    template <int B, unsigned P>
    class Power
    {
    public:
        static const int64_t VALUE = B * Power<B,P-1>::VALUE;
    };

    template <int B>
    class Power<B,0>
    {
    public:
        static const int64_t VALUE = 1;
    };

    template <unsigned P>
    class FloatingNegativePower10
    {
    public:
        static const double DOUBLE_VALUE = FloatingNegativePower10<P-1>::DOUBLE_VALUE / 10;
        static const float FLOAT_VALUE = FloatingNegativePower10<P-1>::FLOAT_VALUE / 10;
    };

    template <>
    class FloatingNegativePower10<0>
    {
    public:
        static const double DOUBLE_VALUE = 1;
        static const float FLOAT_VALUE = 1;
    };

    template <unsigned P>
    class FloatingPositivePower10
    {
    public:
        static const double DOUBLE_VALUE = 10 * FloatingPositivePower10<P-1>::DOUBLE_VALUE;
        static const float FLOAT_VALUE = 10 * FloatingPositivePower10<P-1>::FLOAT_VALUE;
    };

    template <>
    class FloatingPositivePower10<0>
    {
    public:
        static const double DOUBLE_VALUE = 1;
        static const float FLOAT_VALUE = 1;
    };
}

#endif /* PROMOTE_POWER_HPP_ */

