#ifndef PROMOTE_ARRAY_SET_HPP_
#define PROMOTE_ARRAY_SET_HPP_

#include <cassert>
#include <cstdlib>

#include "Iterator.hpp"
#include "PoolAllocator.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename T, bool COPY_SAFE=IsClass<T>::No or IsReference<T>::Yes>
    class ArraySet
    {
    public:
        typedef IndexableIterator<ArraySet<T,COPY_SAFE>,T const> ConstIterator;
        typedef IndexableIterator<ArraySet<T,COPY_SAFE>,T> Iterator;

        ArraySet(std::size_t initCapacity=16)
        : _capacity(initCapacity),
          _size(0),
          _array(new T*[_capacity]),//static_cast<T**>(malloc(_capacity * sizeof(T*)))),
          _pool(sizeof(T), initCapacity)
        { }

        ~ArraySet()
        {
            delete [] _array;
            /*for(std::size_t ii = 0; ii != _size; ++ii) {
                _array[ii]->~T();
            }
            free(_array);*/
        }

        /* Observe */
        bool contains(T const& item) const
        {
            const std::size_t size = _size;
            for(std::size_t ii = 0; ii != size; ++ii) {
                if( *_array[ii] == item ) {
                    return true;
                }
            }
            return false;
        }
        T const& operator[](std::size_t index) const
        {
            return *_array[index];
        }
        bool isEmpty() const        { return _size == 0; }
        std::size_t length() const  { return _size; }
        std::size_t size() const    { return _size; }

        /* Modify */
        bool add(T const& item)
        {
            if( contains(item) ) {
                return false;
            } else if( _size != _capacity ) {
                _array[_size++] = new(_pool.alloc(sizeof(T),false)) T(item);
                return true;
            } else {
                std::size_t const newCapacity(_capacity << 1);
                T** newArray(new T*[newCapacity]);
                assert(newArray);
                memcpy(newArray, _array, _capacity * sizeof(T*));
                _capacity = newCapacity;
                delete [] _array;
                _array = newArray;
                _array[_size++] = new(_pool.alloc(sizeof(T),false)) T(item);
                return true;
            }
        }
        bool remove(T const& item)
        {
            const std::size_t size = _size;
            for(std::size_t ii = 0; ii != size; ++ii) {
                if( *_array[ii] == item ) {
                    _array[ii]->~T();
                    _pool.release(_array[ii]);
                    if( ii != --_size ) {
                        _array[ii] = _array[_size];
                    }
                    _array[_size] = 0;
                    return true;
                }
            }
            return false;
        }
        T& operator[](std::size_t index)
        {
            assert(index < _size);
            return *_array[index];
        }
    private:
        std::size_t _capacity;
        std::size_t _size;
        T** _array;
        PoolAllocator _pool;
    };

    template <typename T>
    class ArraySet<T,true>
    {
    public:
        typedef IndexableIterator<ArraySet<T,true>,T const> ConstIterator;
        typedef IndexableIterator<ArraySet<T,true>,T> Iterator;

        ArraySet(std::size_t initCapacity=16)
        : _capacity(initCapacity),
          _size(0),
          _array(new T[_capacity])//static_cast<T*>(malloc(_capacity * sizeof(T))))
        { }

        ~ArraySet()
        {
            delete [] _array;
            /*for(std::size_t ii = 0; ii != _size; ++ii) {
                _array[ii].~T();
            }
            free(_array);*/
        }

        /* Observe */
        bool contains(T const& item) const
        {
            const std::size_t size = _size;
            for(std::size_t ii = 0; ii != size; ++ii) {
                if( _array[ii] == item ) {
                    return true;
                }
            }
            return false;
        }
        bool isEmpty() const        { return _size == 0; }
        std::size_t length() const  { return _size; }
        std::size_t size() const    { return _size; }
        T const& operator[](std::size_t index) const
        {
            assert(index < _size);
            return _array[index];
        }

        /* Modify */
        bool add(T const& item)
        {
            if( contains(item) ) {
                return false;
            } else if( _size != _capacity ) {
                _array[_size++] = item;
                return true;
            } else {
                std::size_t const newCapacity(_capacity << 1);
                T* newArray(new T[newCapacity]);//static_cast<T*>(realloc(_array, _capacity * sizeof(T)));
                memcpy(newArray, _array, _capacity * sizeof(T));
                _capacity = newCapacity;
                delete [] _array;
                _array = newArray;
                _array[_size++] = item;
                return true;
            }
        }
        bool remove(T const& item)
        {
            const std::size_t size = _size;
            for(std::size_t ii = 0; ii != size; ++ii) {
                if( _array[ii] == item ) {
                    if( ii != (--_size) ) {
                        _array[ii] = _array[_size];
                    }
                    return true;
                }
            }
            return false;
        }
        T& operator[](std::size_t index)
        {
            assert(index < _size);
            return _array[index];
        }
    private:
        std::size_t _capacity;
        std::size_t _size;
        T* _array;
    };
}

#endif /* PROMOTE_ARRAY_SET_HPP_ */

