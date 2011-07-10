#ifndef PROMOTE_EXPANDABLE_ARRAY_HPP
#define PROMOTE_EXPANDABLE_ARRAY_HPP

namespace promote
{
    namespace expandablearray {
        template <typename T, bool COPY=IsClass<T>::No and IsReference<T>::No>
        struct Traits {
            static T* allocate(std::size_t const capacity) {
                return ::new T[capacity];
            }

            static void deallocate(T* const array, T* const end) {
                for(T* const entry = array; entry != end; ++entry) {
                    uninit(entry);
                }
                ::delete [] array;
            }

            static T* expand(T* const array, std::size_t const oldCapacity,
                             std::size_t const newCapacity)
            {
                T* const newArray(allocate(newCapacity));
                for(std::size_t ii = 0; ii != oldCapacity; ++ii) {
                    init(newArray[ii], array[ii]);
                    uninit(array[ii]);
                }
                ::delete [] array;
                return newArray;
            }

            static void init(T& dest, T const& source) {
                new(&dest) T(source);
            }

            static void shift(T* const array, std::size_t const from,
                              std::size_t const to, std::size_t const count)
            {
              if( from < to ) { // right shift
                for(long ii = count - 1; ii != -1; --ii) {
                  uninit(array[to + ii]);
                  init(array[to + ii], array[from + ii]);
                }
                for(std::size_t ii = from; ii != to; ++ii) {
                  uninit(array[ii]);
                }
              } else { // left shift
                for(std::size_t ii = 0; ii != count; ++ii) {
                  uninit(array[to + ii]);
                  init(array[to + ii], array[from + ii]);
                }
                for(std::size_t ii = to; ii != from; ++ii) {
                  uninit(array[ii + count]);
                }
              }
            }

            static void uninit(T& element) {
                element.~T();
            }
        };

        template <typename T>
        struct Traits<T,true> {
            static T* allocate(std::size_t const capacity) {
                return static_cast<T*>(malloc(capacity * sizeof(T)));
            }
            static void deallocate(T* const array, T* const end) {
                free(array);
            }
            static T* expand(T* const array, std::size_t const oldCapacity,
                             std::size_t const newCapacity)
            {
                return static_cast<T*>(realloc(array, newCapacity * sizeof(T)));
            }
            static void init(T& dest, T const& source) {
                dest = source;
            }
            static void shift(T* const array, std::size_t const from,
                              std::size_t const to, std::size_t const count)
            {
              memmove(array + to, array + from, count * sizeof(T));
            }
            static void uninit(T const& element) { }
        };
    }

    template <typename T>
    class ExpandableArray {
    private:
        typedef expandablearray::Traits<T> Traits;
    public:
        ExpandableArray(std::size_t const initCapacity=16);
        ~ExpandableArray();

        // Const
        T const* array() const          { return _array; }
        std::size_t capacity() const    { return _capacity; }

        // Non-Const
        T* array()                      { return _array; }
    private:
        T* _array;
        std::size_t _capacity;
    };
}

#endif /* PROMOTE_EXPANDABLE_ARRAY_HPP */

