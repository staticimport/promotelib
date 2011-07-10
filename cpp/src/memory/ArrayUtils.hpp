#ifndef PROMOTE_ARRAY_UTILS_HPP
#define PROMOTE_ARRAY_UTILS_HPP

#include <new>

#include "Utilities.hpp"

namespace promote {
    template <typename T, bool COPY=IsClass<T>::No and IsReference<T>::No>
    class ArrayUtils {
    public:
        static T* allocateNoInit(std::size_t const capacity) {
            return static_cast<T*>(::operator new(capacity * sizeof(T)));
        }
        static T* allocateInit(std::size_t const capacity) {
            return new T[capacity];
        }
        static void deallocateNoUninit(T* const array) {
            ::operator delete(array);
        }
        static void deallocateUninit(T* const array) {
            delete [] array;
        }
        static T* expand(T* const array, std::size_t const oldCapacity,
                         std::size_t const newCapacity)
        {
            T* const newArray(allocateNoInit(newCapacity));
            for(std::size_t ii = 0; ii != oldCapacity; ++ii) {
                init(newArray[ii], array[ii]);
                uninit(array[ii]);
            }
            deallocateNoUninit(array);
            return newArray;
        }
        static void init(T& dest, T const& source) {
            new(&dest) T(source);
        }
        static void uninit(T& element) {
            element.~T();
        }
    private:
        ArrayUtils() { }
        ArrayUtils(ArrayUtils const& au) { }
    };

    template <typename T>
    class ArrayUtils<T,true> {
    public:
        static T* allocateNoInit(std::size_t const capacity) {
            return static_cast<T*>(malloc(capaciaty * sizeof(T)));
        }
        static T* allocateInit(std::size_t const capacity) {
            return new T[capacity];
        }
        static void deallocateNoUninit(T* const array) {
            free(array);
        }
        static void deallocateUninit(T* const array) {
            delete [] array;
        }
        static T* expand(T* const array, std::size_t const oldCapacity,
                         std::size_t const newCapacity)
        {
            return static_cast<T*>(realloc(array, newCapacity * sizeof(T)));
        }
        static void init(T& dest, T const source) {
            dest = source;
        }
        static void uninit(T& element) { }
    private:
        ArrayUtils() { }
        ArrayUtils(ArrayUtils const& au) { }
    };
}

#endif /* PROMOTE_ARRAY_UTILS_HPP */

