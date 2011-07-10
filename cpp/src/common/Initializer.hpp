#ifndef PROMOTE_INITIALIZER_HPP_
#define PROMOTE_INITIALIZER_HPP_

#include "Utilities.hpp"

namespace promote
{
    template <typename T>
    struct Initializer {
        virtual ~Initializer() { }
        virtual void initialize(T& x) = 0;
    };

    template <typename T, bool INIT_TO_ZERO=IsClass<T>::No>
    struct StandardInitializer : public Initializer<T> {
        static StandardInitializer& instance() {
            static StandardInitializer instance;
            return instance;
        }

        void initialize(T& x) { x = 0; }
    };

    template <typename T>
    struct StandardInitializer<T,false> : public Initializer<T> {
        static StandardInitializer& instance() {
            static StandardInitializer instance;
            return instance;
        }
        void initialize(T& x) { }
    };
}

#endif /* PROMOTE_INITIALIZER_HPP_ */

