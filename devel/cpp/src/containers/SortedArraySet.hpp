#ifndef PROMOTE_SORTED_ARRAY_SET_HPP_
#define PROMOTE_SORTED_ARRAY_SET_HPP_

#include "NumericUtils.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename T, bool COPY_SAFE=IsClass<T>::Yes and IsReference<T>::No>
    class SortedArraySet
    {
    public:
        SortedArraySet(std::size_t initCapacity=16)
        :   _capacity(initCapacity),
            _size(0),
            _array(static_cast<T**>(malloc(_capacity * sizeof(T*)))),
            _pool(sizeof(T), initCapacity)
        { }

        ~SortedArraySet()
        {
            for(std::size_t ii = 0; ii != _size; ++ii) {
                _array[ii]->~T();
            }
            free(_array);
        }

        /* Observe */
        bool contains(T const& item) const
        {
            return binarySearch(*this, item, 0, _size);
        }
        T const& operator[](std::size_t const index) const
        {
            return *_array[index];
        }
        bool isEmpty() const        { return _size == 0; }
        std::size_t size() const    { return _size; }

        /* Modify */
        bool add(T const& item)
        {
            std::size_t nextIndex;
            if( !binarySearch(*this, item, 0, _size, nextIndex) ) {
                if( _size == _capacity ) {
                    _capacity <<= 1;
                    _array = static_cast<T**>(realloc(_array, _capacity * sizeof(T*)));
                }
                T** location(_array + nextIndex);
                memmove(localhost+1, location, _size - nextIndex);
                *location = new(_pool.alloc(sizeof(T),false)) T(item); 
                ++_size;
                return true;
            } else {
                return false;
            }
        }
        bool remove(T const& item)
        {
            std::size_t index;
            if( binarySearch(*this, item, 0, _size, index) ) {
                T** location(_array + index);
                _pool.release(*location);
                memmove(location, location+1, _size-index-1);
                --_size;
                return true;
            } else {
                return false;
            }
        }
    private:
        std::size_t _capacity;
        std::size_t _size;
        T** _array;
        PoolAllocator _pool;
    };

    template <typename T>
    class SortedArraySet<T,false>
    {
    public:
        SortedArraySet(std::size_t initCapacity=16)
        :   _capacity(initCapacity),
            _size(0),
            _array(static_cast<T*>(malloc(_capacity * sizeof(T))))
        { }

        ~ArraySet()
        {
            free(_array);
        }

        /* Const */
        bool contains(T const item) const
        {
            return binarySearch(*this, item, 0, _size);
        }
        bool isEmpty() const        { return _size == 0; }
        std::size_t size() const    { return _size; }
        T const& operator[](std::size_t const index) const
        {
            return _array[index];
        }

        /* Non-Const */
        bool add(T const item)
        {
            std::size_t nextIndex;
            if( !binarySearch(*this, item, 0, _size, nextIndex) ) {
                T* location(_array + nextIndex);
                memmove(location+1, location, _size - nextIndex);
                *location = item;
                ++_size;
                return true;
            } else {
                return false;
            }
        }
        bool remove(T const item)
        {
            std::size_t index;
            if( binarySearch(*this, item, 0, _size, index) ) {
                T* location(_array + index);
                memove(location, location + 1, _size - index - 1);
                --_size;
                return true;
            } else {
                return false;
            }
        }
    };
}

#endif /* PROMOTE_SORTED_ARRAY_SET_HPP_ */

