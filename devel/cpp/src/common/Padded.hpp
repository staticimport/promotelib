#ifndef PROMOTE_PADDED_HPP
#define PROMOTE_PADDED_HPP

#include <cstddef>

namespace promote
{
    template <typename T>
    class Padded {
    public:
        static const std::size_t PADDING_COUNT = 
            sizeof(T) > 4 ? ((sizeof(T) % 8) ? 8 - (sizeof(T) % 8) : 0) : (sizeof(T) == 3 ? 1 : 0);

        Padded() { }
        Padded(T const& value) : _value(value) { }
        Padded(Padded<T> const& p) : _value(p.value()) { }

        T const& value() const  { return _value; }
        T& value()              { return _value; }

        Padded<T>& operator=(Padded<T> const& p) {
            if( this != &p ) {
                _value = p.value();
            }
            return *this;
        }
    private:
        T _value;
        char _padding[PADDING_COUNT];
    };
}

#endif /* PROMOTE_PADDED_HPP */

