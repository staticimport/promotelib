#ifndef PROMOTE_READ_WRITE_RING_BUFFER_HPP
#define PROMOTE_READ_WRITE_RING_BUFFER_HPP

#include <cstddef>
#include <new>

#include "Array.hpp"
#include "Exception.hpp"
#include "Padded.hpp"
#include "Power.hpp"
#include "Structors.hpp"

namespace promote {
    template <typename T, typename STRUCTORS=AutoStructors<T> >
    class ReadWriteRingBuffer {
    public:
        ReadWriteRingBuffer(std::size_t const minCapacity);
        ~ReadWriteRingBuffer();

        // Const
        std::size_t capacity() const { return _array.length(); }
        bool empty(bool reader=false) const {
            if( reader ) {
                std::size_t const read(_readLocalRead);
                std::size_t write(_readLocalWrite);
                if( read == write ) {
                    _readLocalWrite = (write = _volatileWrite);
                }
                return read == write;
            } else {
                return _volatileRead == _volatileWrite;
            }
        }
        bool full(bool writer=false) const { 
            if( writer ) {
                std::size_t read(_writeLocalRead);
                std::size_t const write(_writeLocalWrite);
                if( write - read == _array.length() ) {
                    _writeLocalRead = (read = _volatileRead);
                }
                return write - read == _array.length();
            } else {
                return _volatileWrite - _volatileRead == _array.length();
            }
        }
        T const& peek() const {
            T const* const ptr(tryPeek());
            if( ptr ) {
                return *ptr;
            } else {
                throw Exception("Attempted peek on empty ReadWriteRingBuffer.");
            }
        }
        std::size_t size() const {
            return _volatileWrite - _volatileRead;
        }
        T const* tryPeek() const {
            std::size_t const read(_readLocalRead);
            std::size_t write(_readLocalWrite);
            if( read == write ) {
                _readLocalWrite = (write = _volatileWrite);
            }
            if( read != write ) {
                return &(_array[read & _mask].value());
            } else {
                return NULL;
            }
        }

        // Non-Const
        T& beginPush() {
            T* const ptr(tryBeginPush());
            if( ptr ) {
                return *ptr;
            } else {
                throw Exception("Attempted beginPush on full ReadWriteRingBuffer.");
            }
        }
        void endPush() {
            if( _beganPush ) {
                _volatileWrite = (++_writeLocalWrite);
                _beganPush = false;
            } else {
                throw Exception("Attempted endPush after no beginPush.");
            }
        }
        T& peek() {
            T* const ptr(tryPeek());
            if( ptr ) {
                return *ptr;
            } else {
                throw Exception("Attempted peek on empty ReadWriteRingBuffer.");
            }
        }
        void pop() {
            if( !tryPop() ) {
                throw Exception("Attempted pop on empty ReadWriteRingBuffer.");
            }
        }
        void push(T const& item) {
            if( !tryPush(item) ) {
                throw Exception("Attempted push on full ReadWriteRingBuffer.");
            }
        }
        T* tryPeek() {
            std::size_t const read(_readLocalRead);
            std::size_t write(_readLocalWrite);
            if( read == write ) {
                _readLocalWrite = (write = _volatileWrite);
            }
            if( read != write ) {
                return &(_array[read & _mask].value());
            } else {
                return NULL;
            }
        }
        bool tryPop() {
            std::size_t const read(_readLocalRead);
            std::size_t write(_readLocalWrite);
            if( read == write ) {
                _readLocalWrite = (write = _volatileWrite);
            }
            if( read != write ) {
                STRUCTORS::destruct(_array[read & _mask].value());
                _volatileRead = (_readLocalRead = (read+1));
                return true;
            } else {
                return false;
            }
        }
        T* tryBeginPush() {
            if( _beganPush ) {
                throw Exception("Attempted tryBeginPush when a push has already begun.");
            }
            std::size_t read(_writeLocalRead);
            std::size_t const write(_writeLocalWrite);
            if( write - read == _array.length() ) {
                _writeLocalRead = (read = _volatileRead);
            }
            if( write - read != _array.length() ) {
                _beganPush = true;
                return &(_array[write & _mask].value());
            } else {
                return NULL;
            }
        }
        bool tryPush(T const& item) {
            T* const ptr(tryBeginPush());
            if( ptr ) {
                STRUCTORS::construct(*ptr, item);
                endPush();
                return true;
            } else {
                return false;
            }
        }
    private:
#define CACHE_LINE 64
        // Const members
        Array<Padded<T>,false,IsClass<T>::No and IsReference<T>::No> _array;
        std::size_t const _mask;
        char pad1[CACHE_LINE - sizeof(Padded<T>*) - 2*sizeof(std::size_t)];
        // Volatile counters
        std::size_t volatile _volatileRead;
        std::size_t volatile _volatileWrite;
        char pad2[CACHE_LINE - 2*sizeof(std::size_t)];
        // Reader counters
        std::size_t _readLocalRead;
        mutable std::size_t _readLocalWrite;
        char pad3[CACHE_LINE - 2*sizeof(std::size_t)];
        // Writer counters
        mutable std::size_t _writeLocalRead;
        std::size_t _writeLocalWrite;
        bool _beganPush;
        char pad4[CACHE_LINE - 2*sizeof(std::size_t) - sizeof(bool)];
#undef CACHE_LINE
    };
}

/**
 * Implementation
 **/
template <typename T, typename S>
promote::ReadWriteRingBuffer<T,S>::ReadWriteRingBuffer(std::size_t const minCapacity)
:   _array(static_cast<std::size_t>(promote::leastPower2Above64(minCapacity,true))),
    _mask(_array.length() - 1),
    _volatileRead(0),
    _volatileWrite(0),
    _readLocalRead(0),
    _readLocalWrite(0),
    _writeLocalRead(0),
    _writeLocalWrite(0),
    _beganPush(false)
{
}

template <typename T, typename S>
promote::ReadWriteRingBuffer<T,S>::~ReadWriteRingBuffer()
{
    while( !empty() ) { pop(); }
}

#endif /* PROMOTE_READ_WRITE_RING_BUFFER_HPP */

