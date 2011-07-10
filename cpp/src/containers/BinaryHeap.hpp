#ifndef PROMOTE_BINARY_HEAP_HPP_
#define PROMOTE_BINARY_HEAP_HPP_

#include <cstdio>

#include "Allocator.hpp"
#include "Comparator.hpp"
#include "StandardAllocator.hpp"

namespace promote
{
    namespace binheap {
        template <typename T, bool USE_POINTER=IsClass<T>::Yes>
        class BinaryHeapEntry 
        {
        public:
            typedef T* Type;

            static Type alloc(Allocator& allocator, T const& t)
            {
                return new(allocator.alloc(sizeof(T),false)) T(t);
            }

            static void release(Allocator& allocator, Type const t)
            {
                allocator.release(t);
            }

            static T value(Type const t) { return *t; }
        };

        template <typename T>
        class BinaryHeapEntry<T,false>
        {
        public:
            typedef T Type;

            static Type alloc(Allocator& allocator, T const t)
            {
                return t;
            }

            static void release(Allocator& allocator, Type const t)
            {
            }
            
            static T value(Type const t) { return t; }
        };

        template <typename T, bool MIN=true, bool USE_COMPARATOR=false>
        class Ordering
        {
        public:
            static bool isOrderCorrect(typename Passable<T const>::Type a, 
                                       typename Passable<T const>::Type b, 
                                       Comparator<T> const& comp)
            {
                return a <= b;
            }
        };

        template <typename T>
        class Ordering<T,true,true>
        {
        public:
            static bool isOrderCorrect(typename Passable<T const>::Type a,
                                       typename Passable<T const>::Type b, 
                                       Comparator<T> const& comp)
            {
                return (comp.compare(a,b) != ORDER_DECREASING);
            }
        };

        template <typename T>
        class Ordering<T,false,false>
        {
        public:
            static bool isOrderCorrect(typename Passable<T const>::Type a, 
                                       typename Passable<T const>::Type b, 
                                       Comparator<T> const& comp)
            {
                return a >= b;
            }
        };

        template <typename T>
        class Ordering<T,false,true>
        {
        public:
            static bool isOrderCorrect(typename Passable<T const>::Type a, 
                                       typename Passable<T const>::Type b, 
                                       Comparator<T> const& comp)
            {
                return (comp.compare(a,b) != ORDER_INCREASING);
            }
        };
    }

    template <typename T, bool MIN=true, bool USE_COMPARATOR=false>
    class BinaryHeap
    {
    private:
        typedef typename binheap::BinaryHeapEntry<T>::Type Entry;
    public:
        static std::size_t const ALLOC_SIZE = sizeof(T);

        BinaryHeap(std::size_t initCapacity=16,
                   Comparator<T> const& comparator=StandardComparator<T>::instance())
        :   _allocator(StandardAllocator::instance()),
            _comparator(comparator),
            _array(static_cast<Entry*>(malloc((initCapacity+1) * sizeof(Entry)))),
            _capacity(initCapacity),
            _size(0)
        {
        }

        BinaryHeap(Allocator& allocator,
                   std::size_t initCapacity=16,
                   Comparator<T> const& comparator=StandardComparator<T>::instance())
        :   _allocator(allocator),
            _comparator(comparator),
            _array(static_cast<Entry*>(malloc((initCapacity+1) * sizeof(Entry)))),
            _capacity(initCapacity),
            _size(0)
        {
        }

        ~BinaryHeap()
        {
            for(std::size_t ii = 1; ii <= _size; ++ii) {
                binheap::BinaryHeapEntry<T>::release(_allocator, _array[ii]);
            }
            free(_array);
        }

        /* Const */
        bool empty() const          { return (_size == 0); }
        Entry const& peek() const   { return *_array[1]; }
        std::size_t size() const    { return _size; }

        /* Non-Const */
        void add(T const& x)
        {
            // Allocate and New up
            Entry const entry(binheap::BinaryHeapEntry<T>::alloc(_allocator, x));

            // Check if array must be expanded
            if( _size == _capacity ) {
                _capacity <<= 1;
                _array = static_cast<Entry*>(realloc(_array, (_capacity+1) * sizeof(Entry)));
            }

            // Update position
            pushUp(entry, ++_size);
        }
        typename Passable<T>::Type peek() { return value(_array[1]); }
        void pop()
        {
            binheap::BinaryHeapEntry<T>::release(_allocator, _array[1]);
            pushDown(_array[_size--], 1);
        }
    private:
        bool isOrderCorrect(T const& a, T const& b)
        {
            return binheap::Ordering<T,MIN,USE_COMPARATOR>::isOrderCorrect(a,b,_comparator);
        }

        void pushDown(Entry const entry, std::size_t index)
        {
            T const entryValue(value(entry));

            std::size_t left(index << 1);
            std::size_t right(left + 1);
            std::size_t swap(index);
            while( left <= _size ) {
                if( left != _size ) {
                    // Right is valid too
                    if( !isOrderCorrect(entryValue, value(_array[left])) ) {
                        swap = isOrderCorrect(value(_array[left]), value(_array[right])) ? left : right;
                    } else if( !isOrderCorrect(entryValue, value(_array[right])) ) {
                        swap = right;
                    }
                } else if( !isOrderCorrect(entryValue, value(_array[left])) ) {
                    swap = left;
                }

                if( index != swap ) {
                    _array[index] = _array[swap];
                    index = swap;
                    right = (left = (index << 1)) + 1;
                } else {
                    break;
                }
            }

            _array[index] = entry;
        }

        void pushUp(Entry const entry, std::size_t index)
        {
            T const entryValue(value(entry));

            std::size_t parent(index >> 1);
            while( parent ) {
                Entry const parentEntry(_array[parent]);
                if( !isOrderCorrect(value(parentEntry), entryValue) ) {
                    _array[index] = parentEntry;
                    parent = (index = parent) >> 1;
                } else {
                    break;
                }
            }
            _array[index] = entry;
        }

        static T value(Entry const entry)
        {
            return binheap::BinaryHeapEntry<T>::value(entry);
        }

        Allocator& _allocator;
        Comparator<T> const& _comparator;
        Entry* _array;
        std::size_t _capacity;
        std::size_t _size;
    };
}

#endif /* PROMOTE_BINARY_HEAP_SET_HPP_ */

