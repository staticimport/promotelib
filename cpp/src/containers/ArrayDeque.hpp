#ifndef PROMOTE_ARRAY_DEQUE_HPP_
#define PROMOTE_ARRAY_DEQUE_HPP_

#include <cassert>
#include <cstdlib>
#include <cstring>

#include "Iterator.hpp"
#include "Power.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename T, bool NEEDS_CONSTRUCTOR> class ArrayDequeManipulator;

    template <typename T, bool NEEDS_CONSTRUCTOR=IsClass<T>::Yes>
    class ArrayDeque
    {
    public:
        /***
         * Iterators
         ***/
        typedef IndexableIterator<ArrayDeque<T> const,T const> ConstIterator;
        typedef IndexableIterator<ArrayDeque<T>,T> Iterator;

        ArrayDeque(const std::size_t minCapacity=16) :
            _capacity(static_cast<std::size_t>(leastPower2Above64(static_cast<uint64_t>(minCapacity),true))),
            _mask(_capacity - 1),
            _array(new T[_capacity]),//static_cast<T*>(malloc(_capacity * sizeof(T)))),
            _front(0),
            _size(0)
        { }

        ~ArrayDeque()
        {
            destroy(_array);
        }

        /* Const */
        std::size_t capacity()  const { return _capacity; }
        bool isEmpty()          const { return _size == 0; }
        std::size_t length()    const { return _size; }
        std::size_t size()      const { return _size; }
        T const& operator[](std::size_t index) const
        {
            assert(index < _size);
            return _array[(_front + index) & _mask];
        }

        /* Non-Const */
        bool pushBack(T const& item)
        {
            if( _size != _capacity or expand() ) {
                _array[(_front + _size++) & _mask] = item;
                return true;
            } else {
                return false;
            }
        }
        bool pushFront(T const& item)
        {
            if( _size != _capacity or expand() ) {
                _array[_front = ((_front - 1) & _mask)] = item;
                ++_size;
                return true;
            } else {
                return false;
            }
        }
        void reserve(std::size_t const capacity) {
            if( capacity > _capacity ) {
                assert(expand(capacity));
            }
        }
        bool set(T const& item, const std::size_t index)
        {
            if( index < _size ) {
                _array[(_front + index) & _mask] = item;
                return true;
            } else if( index == size )
                return pushFront(item);
            else
                return false;
        }
        T& operator[](std::size_t index)
        {
            assert(index < _size);
            return _array[(_front + index) & _mask];
        }
    private:
        friend class ArrayDequeManipulator<T,NEEDS_CONSTRUCTOR>;
        //friend template <typename A, bool B> ArrayDequeManipulator<A,B>;

        bool expand() { return expand(_capacity << 1); }
        bool expand(std::size_t const minCapacity);
        
        static void destroy(T* const _array);

        std::size_t _capacity;
        std::size_t _mask;
        T* _array;
        std::size_t _front;
        std::size_t _size;
    };
    
    template <typename T, bool NEEDS_CONSTRUCTOR=true>
    class ArrayDequeManipulator {
    private:
        friend class ArrayDeque<T,NEEDS_CONSTRUCTOR>;

        static void destroy(T* const array) { delete [] array; }

        static bool expand(ArrayDeque<T,NEEDS_CONSTRUCTOR>& list, std::size_t const minCapacity) {
            std::size_t const minCapacitySzt(static_cast<uint64_t>(minCapacity));
            std::size_t const newCapacity = 
                static_cast<std::size_t>(leastPower2Above64(minCapacitySzt,true));
            const std::size_t newMask = newCapacity - 1;
            assert((newCapacity & (newCapacity - 1)) == 0);
            T* const newArray = new T[newCapacity];
            if( newArray ) {
                for(std::size_t ii = 0; ii != list._size; ++ii) {
                    newArray[ii] = list[ii];
                }
                destroy(list._array);
                list._array = newArray;
                list._capacity = newCapacity;
                list._mask = newMask;
                list._front = 0;
                return true;
            } else {
                return false;
            }
        }
    };
    
    template <typename T>
    class ArrayDequeManipulator<T,false> {
    private:
        friend class ArrayDeque<T,false>;

        static void destroy(T* const array) { free(array); }

        static bool expand(ArrayDeque<T,false>& list, std::size_t const minCapacity) {
            std::size_t const minCapacitySzt(static_cast<uint64_t>(minCapacity));
            std::size_t const newCapacity = 
                static_cast<std::size_t>(leastPower2Above64(minCapacitySzt,true));
            const std::size_t newMask = newCapacity - 1;
            assert((newCapacity & (newCapacity - 1)) == 0);
            T* const newArray = static_cast<T*>(malloc(newCapacity * sizeof(T)));
            if( newArray ) {
                const std::size_t frontIndex = list._front & list._mask;
                const std::size_t afterFront = list._capacity - frontIndex;
                if( afterFront >= list._size )
                    memcpy(newArray, list._array + frontIndex, list._size * sizeof(T));
                else {
                    memcpy(newArray, list._array + frontIndex, afterFront * sizeof(T));
                    memcpy(newArray + afterFront, list._array, (list._size - afterFront) * sizeof(T));
                }
                destroy(list._array);
                list._array = newArray;
                list._capacity = newCapacity;
                list._mask = newMask;
                list._front = 0;
                return true;
            } else {
                return false;
            }
        }
    };
}

template <typename T, bool NEEDS_CONSTRUCTOR>
bool promote::ArrayDeque<T,NEEDS_CONSTRUCTOR>::expand(std::size_t const minCapacity)
{
    return ArrayDequeManipulator<T,NEEDS_CONSTRUCTOR>::expand(*this, minCapacity);
}

template <typename T, bool NEEDS_CONSTRUCTOR>
void promote::ArrayDeque<T,NEEDS_CONSTRUCTOR>::destroy(T* const array)
{
    ArrayDequeManipulator<T,NEEDS_CONSTRUCTOR>::destroy(array);
}


#endif /* PROMOTE_ARRAY_DEQUE_HPP_ */

