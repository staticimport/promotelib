#ifndef PROMOTE_FRACTION_HPP_
#define PROMOTE_FRACTION_HPP_

#include <stdint.h>

#include "NumericUtils.hpp"

namespace promote
{
    template <typename T=int64_t>
    class Fraction
    {
    public:
        Fraction(Fraction const& fraction);
        Fraction(T const numerator,
                 T const denominator);

        /* Const */
        T denominator() const   { return _denominator; }
        T numerator() const     { return _numerator; }
        template <typename V>
        V value() const
        {
            return static_cast<V>(_numerator) / static_cast<V>(_denominator);
        }

        Fraction pow(int const power)
        {
            if( power > 0 ) {
                return Fraction(uintPow(_numerator), uintPow(_denominator));
            } else {
                return Fraction(uintPow(_numerator), uintPow(_denominator));
            }
        }
        Fraction operator+(Fraction const& other) const
        {
            return Fraction((_numerator * other.denominator()) + (other.numerator() * _denominator),
                            _denominator * other.denominator);
        }
        Fraction operator-(Fraction const& other) const
        {
            return Fraction((_numerator * other.denominator()) - (other.numerator() * _denominator),
                            _denominator * other.denominator);
        }
        Fraction operator*(Fraction const& other) const
        {
            return Fraction(_numerator * other.numerator(),
                            _denominator * other.denominator());
        }
        Fraction operator/(Fraction const& other) const
        {
            return Fraction(_numerator * other.denominator(),
                            _denominator * other.numerator());
        }

        /* Comparison */
        bool operator==(Fraction const& other) const
        {
            return _numerator == other.numerator() and _denominator == other.denominator();
        }
        bool operator!=(Fraction const& other) const
        {
            return !((*this) == other);
        }
        bool operator<(Fraction const& other) const
        {
            return (_numerator * other.denominator()) < (_denominator * other.numerator());
        }
        bool operator<=(Fraction const& other) const
        {
            return (_numerator * other.denominator()) <= (_denominator * other.numerator());
        }
        bool operator>(Fraction const& other) const
        {
            return (_numerator * other.denominator()) > (_denominator * other.numerator());
        }
        bool operator>=(Fraction const& other) const
        {
            return (_numerator * other.denominator()) >= (_denominator * other.numerator());
        }
    private:
        T _numerator;
        T _denominator;
    };
}

/***
 * Implementation
 ***/
template <typename T>
promote::Fraction<T>::Fraction(promote::Fraction<T> const& fraction)
:   _numerator(fraction.numerator()),
    _denominator(fraction.denominator())
{
}

template <typename T>
promote::Fraction<T>::Fraction(T const numerator,
                               T const denominator)
:   _numerator(numerator),
    _denominator(denominator)
{
    if( denominator < 0 ) {
        _numerator = -_numerator;
        _denominator = -_denominator;
    }
    T const gcd(gcd<T>(_numerator, _denominator));
    _numerator /= gcd;
    _denominator /= gcd;
}

#endif /* PROMOTE_FRACTION_HPP_ */

