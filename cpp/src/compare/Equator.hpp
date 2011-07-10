#ifndef PROMOTE_EQUATOR_HPP_
#define PROMOTE_EQUATOR_HPP_

#include "Utilities.hpp"

namespace promote 
{
    template <typename T>
    //bool equal(typename Passable<T const>::Type a, 
    //           typename Passable<T const>::Type b)
    bool equal(T const& a, T const& b)
    {
        return a == b;
    }

    template <typename T>
    class Equator
    {
    public:
        virtual ~Equator() { }
        virtual bool equal(T const& a, T const& b) const = 0;
    };

    template <typename T>
    class StandardEquator : Equator<T>
    {
    public:
        StandardEquator()  { }
        ~StandardEquator() { }

        static const StandardEquator<T>& instance()
        {
            static StandardEquator<T>& instance;
            return instance;
        }

        bool equal(T const& a, T const& b) const
        {
            return a == b;
        }
    };
}

#endif /* PROMOTE_EQUATOR_HPP_ */

