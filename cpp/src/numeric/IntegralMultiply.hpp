#ifndef PROMOTE_INTEGRAL_MULTIPLICATION_HPP_
#define PROMOTE_INTEGRAL_MULTIPLICATION_HPP_

#include "Utilities.hpp"

namespace promote
{
    template <typename T, std::size_t FACTOR>
    class IntegralMultiply
    {
    public:
        static T const FACTOR_T = static_cast<T>(FACTOR);

        static T multiply(T const n) { return n * FACTOR_T; }
    };

    /***
     * Powers of 2
     ***/
    template <typename T>
    class IntegralMultiply<T,2>
    {
    public:
        static T multiply(T const n) { return n << 1; }
    };
    template <typename T>
    class IntegralMultiply<T,4>
    {
    public:
        static T multiply(T const n) { return n << 2; }
    };
    template <typename T>
    class IntegralMultiply<T,8>
    {
    public:
        static T multiply(T const n) { return n << 3; }
    };
    template <typename T>
    class IntegralMultiply<T,16>
    {
    public:
        static T multiply(T const n) { return n << 4; }
    };
    template <typename T>
    class IntegralMultiply<T,32>
    {
    public:
        static T multiply(T const n) { return n << 5; }
    };
    template <typename T>
    class IntegralMultiply<T,64>
    {
    public:
        static T multiply(T const n) { return n << 6; }
    };

    /***
     * Powers of 10
     ***/
    template <typename T>
    class IntegralMultiply<T,10>
    {
    public:
        static T multiply(T const n) 
        { 
            T const n8(n << 3);
            T const n2(n << 1);
            return n8 + n2;
        }
    };
    template <typename T>
    class IntegralMultiply<T,100>
    {
    public:
        static T multiply(T const n) 
        { 
            T const n64(n << 6);
            T const n32(n << 5);
            T const n2(n << 1);
            return n64 + n32 + n2;
        }
    };
    template <typename T>
    class IntegralMultiply<T,1000>
    {
    public:
        static T multiply(T const n) 
        { 
            T const n1024(n << 10);
            T const n32(n << 5);
            T const n8(n << 3);
            return (n1024 - n32) + n8;
        }
    };

}

#endif /* PROMOTE_INTEGRAL_MULTIPLICATION_HPP_ */

