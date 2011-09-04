#ifndef PROMOTE_STACK_HPP_
#define PROMOTE_STACK_HPP_

#include <cstdlib>

#include "PromoteException.hpp"

namespace promote
{
    template <typename T>
    class Stack
    {
    private:
        class Segment
        {
        public:
            Segment(const std::size_t capacity, Segment* const previous)
            :   _array(static_cast<T*>(malloc(capacity * sizeof(T)))),
                _capacity(capacity),
                _next(0),
                _previous(previous)
            { }

            ~Segment()
            {
                free(_array);
            }

            std::size_t capacity() const
            {
                return _capacity;
            }

            bool hasTop() const
            {
                return (_next != 0);
            }

            bool pop()
            {
                if( _next ) {
                    _array[--_next].~T();
                    return true;
                } else {
                    return false;
                }
            }

            Segment* previous()
            {
                return _previous;
            }

            void previous(Segment* const newPrevious)
            {
                _previous = newPrevious;
            }

            bool push(T const& item)
            {
                if( _next != _capacity ) {
                    _array[_next++] = item;
                    return true;
                } else {
                    return false;
                }
            }

            T const& top() const
            {
                return _array[_next-1];
            }

            T& top()
            {
                return _array[_next-1];
            }
        private:
            T* _array;
            std::size_t _capacity;
            std::size_t _next;
            Segment* _previous;
        };

    public:
        static const std::size_t ALLOC_SIZE = sizeof(Segment);

        Stack(const std::size_t initCapacity=16)
        :   _topSegment(new Segment(initCapacity, 0)), _freeSegments(0)
        { }

        ~Stack()
        {
            Segment* segment(_topSegment);
            while( segment ) {
                Segment* previous(segment->previous());
                delete segment;
                segment = previous;
            }
        }

        void clear()
        {
            while( pop() ) ;
        }

        bool empty() const
        {
            return !_topSegment->hasTop() and (_topSegment->previous() == 0);
        }

        bool pop()
        {
            if( _topSegment->pop() ) {
                return true;
            } else { 
                Segment* previous(_topSegment->previous());
                if( previous ) {
                    _topSegment->previous(_freeSegments);
                    _freeSegments = _topSegment;
                    _topSegment = previous;
                    return previous->pop();
                } else {
                    return false;
                }
            }
        }

        bool push(T const& item)
        {
            if( _topSegment->push(item) ) {
                return true;
            } else if( _freeSegments ) {
                Segment* newTop(_freeSegments);
                _freeSegments = newTop->previous();
                newTop->previous(_topSegment);
                _topSegment = newTop;
                return newTop->push(item);
            } else {
                _topSegment = new Segment(_topSegment->capacity() << 1, _topSegment);
                return _topSegment->push(item);
            }
        }

        T const& top() const
        {
            if( _topSegment->hasTop() ) {
                return _topSegment->top();
            } else {
                Segment* previous(_topSegment->previous());
                if( previous ) {
                    _topSegment->previous(_freeSegments);
                    _freeSegments = _topSegment;
                    _topSegment = previous;
                    return previous->top();
                } else {
                    throw PromoteException("Stack is empty!");
                }
            }
        }

        T& top()
        {
            if( _topSegment->hasTop() ) {
                return _topSegment->top();
            } else {
                Segment* previous(_topSegment->previous());
                if( previous ) {
                    _topSegment->previous(_freeSegments);
                    _freeSegments = _topSegment;
                    _topSegment = previous;
                    return previous->top();
                } else {
                    throw PromoteException("Stack is empty!");
                }
            }
        }
    private:
        Segment* _topSegment;
        Segment* _freeSegments;
    };
}

#endif /* PROMOTE_STACK_HPP_ */

