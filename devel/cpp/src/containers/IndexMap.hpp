#ifndef PROMOTE_INDEX_MAP_HPP_
#define PROMOTE_INDEX_MAP_HPP_

#include "Array.hpp"
#include "Exception.hpp"
#include "Initializer.hpp"
#include "PoolQueueAllocator.hpp"

namespace promote
{
    template <typename T>//, bool NEEDS_CONSTRUCTOR=IsClass<T>::Yes>
    class IndexMap {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        IndexMap(std::size_t const initCapacity=16,
                 Initializer<T>& initializer=StandardInitializer<T>::instance());
        ~IndexMap();

        // Const
        bool capacity() const                   { return _capacity; }
        T const* find(std::size_t const index) const {
            return index < _capacity ? &(_array[index]) : 0;
        }
        T const& operator[](std::size_t const index) const {
            T const* ptr(find(index));
            if( ptr ) {
                return *ptr;
            } else {
                throw Exception("Index not contained in IndexMap.");
            }
        }
        /*bool contains(std::size_t index) const { return _size > index; }
        bool empty() const                     { return _size == 0; }
        T const* find(std::size_t const index) const {
            if( contains(index) ) {
                Segment const* segment(_head);
                while( index >= segment->sizeUsed ) {
                    index -= segment->sizeUsed;
                    segment = segment->next;
                }
                return &(segment->array[index]);
            } else {
                return 0;
            }
        }
        std::size_t size() const               { return _size; }
        T const& operator[](std::size_t const index) const {
            T const* ptr(find(index));
            if( ptr ) {
                return *ptr;
            } else {
                throw Exception("Index not contained in IndexMap.");
            }
        }

        // Non-Const
        T& operator[](std::size_t index) {
            if( index < _size ) {
                Segment* segment(_head);
                while( index >= segment->array.length() ) {
                    index -= segment->array.length();
                    segment = segment->next;
                }
                return segment->array[index];
            } else {
                Segment* segment(_tail);
                for(std::size_t ii = _size; ii <= index; ++ii) {
                    if( segment->sizeUsed == segment->array.length() ) {
                        // New Segment
                        void* const segmentSpace(_segmentAllocator.alloc(sizeof(Segment),false));
                        Segment* const newSegment(new(segmentSpace) Segment(segment->array.length() << 1));
                        segment->next = newSegment;
                        _tail = segment = newSegment;
                    }
                    _initializer.initialize(segment->array[segment->sizeUsed++]);
                }
                _size = index+1;
                return segment->array[segment->sizeUsed-1];
            }
        }*/
        T& operator[](std::size_t const index) {
            if( index >= _capacity ) {
                std::size_t const newCapacity(std::max(index+1, _capacity << 1));
                _array = static_cast<T*>(realloc(_array, newCapacity * sizeof(T)));
                for(std::size_t ii = _capacity; ii != newCapacity; ++ii) {
                    _initializer.initialize(_array[ii]);
                }
                _capacity = newCapacity;
            }
            return _array[index];
        }
    private:
        struct Segment {
            Segment(std::size_t const capacity) 
            : array(capacity), sizeUsed(0), next(0)
            { }

            promote::Array<T> array;
            std::size_t sizeUsed;
            Segment* next;
        };

        Initializer<T>& _initializer;
        PoolQueueAllocator<false,false> _segmentAllocator;
        T* _array;
        std::size_t _capacity;
        Segment* const _head;
        Segment* _tail;
        std::size_t _size;
    };
}

/***
 * Implementation
 ***/
template <typename T>
promote::IndexMap<T>::IndexMap(std::size_t const initCapacity,
                               Initializer<T>& initializer)
:   _initializer(initializer),
    _segmentAllocator(sizeof(Segment), 8),
    _array(static_cast<T*>(malloc(sizeof(T) * initCapacity))),
    _capacity(initCapacity),
    _head(new(_segmentAllocator.alloc(sizeof(Segment),false)) Segment(initCapacity)),
    _tail(_head),
    _size(0)
{
    for(std::size_t ii = 0; ii != _capacity; ++ii) {
        _initializer.initialize(_array[ii]);
    }
}

template <typename T>
promote::IndexMap<T>::~IndexMap() {
    Segment* segment(_head);
    while( segment ) {
        Segment* const next(segment->next);
        segment->~Segment();
        _segmentAllocator.release(segment);
        segment = next;
    }
    for(std::size_t ii = 0; ii != _capacity; ++ii) {
        //_array[ii].~T();
    }
    free(_array);
}

#endif /* PROMOTE_INDEX_MAP_HPP_ */

