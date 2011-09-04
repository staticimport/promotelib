#ifndef PROMOTE_BUFFER_QUEUE_HPP_
#define PROMOTE_BUFFER_QUEUE_HPP_

#include <cassert>
#include <cstring>

namespace promote
{
    template <typename T>
    class Buffer
    {
    public:
        Buffer(unsigned capacity, double fullShiftThreshold=0.8);
        ~Buffer();

        /* Const */
        int available() const   { return _capacity - _tail; }
        T* const buffer() const { return _buffer; }
        int buffered() const    { return _tail - _head; }
        int capacity() const    { return _capacity; }
        T* const head() const   { return _buffer + _head; }
        bool isEmpty() const    { return _head == _tail; }
        T* const tail() const   { return _buffer + _tail; }

        /* Non-Const */
        void add(int n, bool shiftIfNeeded=true)
        {
            assert(n <= available());
            _tail += n;
            if( shiftIfNeeded and _tail > _threshold ) {
                shift();
            }
        }
        void remove(int n, bool shiftIfNeeded=true)
        {
            assert(n <= buffered());
            _head += n;
            if( _head == _tail ) {
                _head = _tail = 0;
            } else if( shiftIfNeeded and _tail > _threshold ) {
                shift();
            }
        }
        void shift()
        {
            int const size(_tail - _head);
            memmove(_buffer, _buffer + _head, size * sizeof(T));
            _head = 0;
            _tail = size;
        }
    private:
        T* const _buffer;
        const int _capacity;
        const int _threshold;
        int _head;
        int _tail;
    };
}

/***
 * Implementation
 ***/
template <typename T>
promote::Buffer<T>::Buffer(unsigned capacity,
                           double fullShiftThreshold)
:   _buffer(new T[capacity]),
    _capacity(capacity),
    _threshold(static_cast<int>(capacity * fullShiftThreshold)),
    _head(0),
    _tail(0)
{
}

template <typename T>
promote::Buffer<T>::~Buffer()
{
    delete [] _buffer;
}

#endif /* PROMOTE_BUFFER_QUEUE_HPP_ */

