#ifndef PROMOTE_COMPARATOR_HPP_
#define PROMOTE_COMPARATOR_HPP_

#include "Equator.hpp"
#include "Utilities.hpp"

namespace promote
{
    enum Order { ORDER_INCREASING, ORDER_SAME, ORDER_DECREASING };

    template <typename T>
    //Order compare(typename Passable<T const>::Type a,  
    //              typename Passable<T const>::Type b)
    Order compare(T const& a, T const& b)
    {
        if( a < b ) {
            return ORDER_INCREASING;
        } else if( !(a == b) ) {
            return ORDER_DECREASING;
        } else {
            return ORDER_SAME;
        }
    }

    template <typename T>
    class Comparator : public Equator<T>
    {
    public:
        virtual ~Comparator() { }
        virtual Order compare(T const& a, T const& b) const = 0;

        bool equal(T const& a, T const& b) const
        {
            return compare(a,b) == ORDER_SAME;
        }
    };

    template <typename T, bool REAL=true>
    class StandardComparator : public Comparator<T>
    {
    public:
        StandardComparator()  { }
        ~StandardComparator() { }

        Order compare(T const& a, T const& b) const
        {
            return promote::compare<T>(a,b);
        }

        static const StandardComparator<T,true>& instance()
        {
            static StandardComparator<T,true> instance;
            return instance;
        }
    };

    template <typename T>
    class StandardComparator<T,false> : public Comparator<T>
    {
    public:
        StandardComparator()  { }
        ~StandardComparator() { }

        Order compare(T const& a, T const& b) const
        {
            return ORDER_SAME;
        }

        static const StandardComparator<T,false>& instance()
        {
            static StandardComparator<T,false> instance;
            return instance;
        }
    };
}

#endif /* PROMOTE_COMPARATOR_HPP_ */

