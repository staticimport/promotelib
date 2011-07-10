#ifndef PROMOTE_STRUCTORS_HPP
#define PROMOTE_STRUCTORS_HPP

#include "Utilities.hpp"

namespace promote {
    template <typename T>
    class RealStructors {
    public:
        static void construct(T& toConstruct, T const& toCopy) {
            new(&toConstruct) T(toCopy);
        }
        static void destruct(T& toDestruct) {
            toDestruct.~T();
        }
    private:
        RealStructors() { }
        RealStructors(RealStructors const& s) { }
    };

    template <typename T>
    class PrimitiveStructors {
    public:
        static void construct(T& toConstruct, typename Passable<T>::Type toCopy) {
            toConstruct = toCopy;
        }
        static void destruct(typename Passable<T>::Type toDestruct) { }
    private:
        PrimitiveStructors() { }
        PrimitiveStructors(PrimitiveStructors const& s) { }
    };

    template <typename T, bool REAL_STRUCTORS=IsClass<T>::Yes or IsReference<T>::Yes>
    class AutoStructors {
    public:
        static void construct(T& toConstruct, T const& toCopy) {
            RealStructors<T>::construct(toConstruct, toCopy);
        }
        static void destruct(T& toDestruct) {
            RealStructors<T>::destruct(toDestruct);
        }
    private:
        AutoStructors() { }
        AutoStructors(AutoStructors const& s) { }
    };

    template <typename T>
    class AutoStructors<T,false> {
    public:
        static void construct(T& toConstruct, typename Passable<T const>::Type toCopy) {
            PrimitiveStructors<T>::construct(toConstruct, toCopy);
        }
        static void destruct(typename Passable<T>::Type toDestruct) {
            PrimitiveStructors<T>::destruct(toDestruct);
        }
    private:
        AutoStructors() { }
        AutoStructors(AutoStructors const& s) { }
    };
}

#endif /* PROMOTE_STRUCTORS_HPP */

