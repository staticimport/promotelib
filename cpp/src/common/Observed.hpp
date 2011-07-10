#ifndef PROMOTE_OBSERVERED_HPP_
#define PROMOTE_OBSERVERED_HPP_

#include "Utilities.hpp"

namespace promote
{
    template <typename T> class Observed; // Defined below

    template <typename T>
    struct Observer {
        virtual ~Observer() { }

        virtual void onConstructor(Observed<T>& observed) = 0;
        virtual void onDestructor(Observed<T>& observed) = 0;
    };

    template <typename T>
    class Observed {
    public:
        Observed(typename promote::Passable<T const>::Type value,
                 Observer<T>& observer)
        :   _observer(observer), _value(value)
        {
            _observer.onConstructor(*this);
        }
        Observed(Observed const& observed)
        :   _observer(observed._observer), _value(observed._value)
        {
            _observer.onConstructor(*this);
        }

        ~Observed() { _observer.onDestructor(*this); }

        // Const
        typename promote::Passable<T const>::Type value() const { return _value; }

        // Non-Const
        typename promote::Passable<T>::Type value() { return _value; }
    private:
        Observer<T>& _observer;
        T _value;
    };
}

#endif /* PROMOTE_OBSERVERED_HPP_ */

