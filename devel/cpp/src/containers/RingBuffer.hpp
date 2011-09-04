#ifndef PROMOTE_RING_BUFFER_HPP
#define PROMOTE_RING_BUFFER_HPP

#include "ArrayUtils.hpp"

namespace promote
{
    template <typename T>
    class RingBuffer {
    private:
        typedef ArrayUtils<Padded<T>,IsClass<T>::No and IsReference<T>::No> ArrayUtils;
    public:
        struct Exception : public::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        RingBuffer(std::size_t const minCapacity);
        ~RingBuffer();

        // Const
        bool empty() const          { return _read == _write; }
        bool full() const           { return _write - _read == _capacity; }
        T const& peek() const {
            assert(!empty());
            return _array[_read & _mask].value();
        }
        std::size_t size() const    { return _write - _read; }

        // Non-Const
        T& beginPush() {
            assert(!full());
            return *tryBeginPush();
        }
        void endPush() {
            assert(!full());
            ++_write;
        }
        T& peek() {
            assert(!empty());
            return _array[_read & _mask].value();
        }
        void pop() { 
            assert(!empty());
            ArrayUtils::uninit(_array[_read & _mask]);
            ++_read;
        }
        void push(T const& item) {
            assert(!full());
            if( !tryPush(item) ) {
                throw Exception("Attempted push on full RingBuffer.");
            }
        }
        T* tryBeginPush() {
            std::size_t const write(_write);
            if( write - _read != _capacity ) {
                return &(_array[write & _mask]);
            } else {
                return NULL;
            }
        }
        bool tryPush(T const& item) {
            std::size_t const write(_write);
            if( write - _read != _capacity ) {
                ArrayUtils::init(_array[write & _mask], item);
                _write = write + 1;
                return true;
            } else {
                return false;
            }
        }
    private:
        Padded<T>* const _array;
        std::size_t const _capacity;
        std::size_t const _mask;
        std::size_t _read;
        std::size_t _write;
    };

}

#endif /* PROMOTE_RING_BUFFER_HPP */

