#ifndef PROMOTE_ARRAY_HPP_
#define PROMOTE_ARRAY_HPP_

#include <cassert>
#include <cstddef>
#include <iostream>

#include "Padded.hpp"
#include "Utilities.hpp"

namespace promote
{
    namespace array {
        template <typename T, bool PADDED>
        struct Traits {
            typedef T Entry;
            static T& value(Entry& e)               { return e; }
            static T const& value(Entry const& e)   { return e; }
        };

        template <typename T>
        struct Traits<T,true> {
            typedef Padded<T> Entry;
            static T& value(Entry& e)               { return e.value(); }
            static T const& value(Entry const& e)   { return e.value(); }
        };
    }

    template <typename T, bool PADDED=false, bool CTOR_INIT=true, bool COPY_SAFE=IsClass<T>::No and IsReference<T>::No>
    class Array {
    private:
        typedef array::Traits<T,PADDED> Traits;
    public:
        typedef typename Traits::Entry Entry;

        Array(std::size_t const length);
        ~Array();

        // Const
        Entry const* begin() const                  { return _array; }
        Entry const* end() const                    { return _end; }
        std::size_t length() const                  { return _length; }
        T const& value(Entry const& entry) const    { return Traits::value(entry); }
        T const& operator[](std::size_t const index) const {
            //assert(index < _length);
            return Traits::value(_array[index]);
        }

        // Non-Const
        Entry* begin()                              { return _array; }
        Entry* end()                                { return _end; }
        void reserve(std::size_t const minLength) {
            if( minLength > _length ) {
                resize(minLength);
            }
        }
        void resize(std::size_t const length);
        T& value(Entry& entry) const                { return Traits::value(entry); }
        T& operator[](std::size_t const index) {
            //assert(index < _length);
            return Traits::value(_array[index]);
        }
    private:
        Entry* _array;
        Entry* _end;
        std::size_t _length;
    };

    template <typename T, std::size_t L, bool CTOR_INIT=true>
    class StaticArray
    {
    public:
        StaticArray() : _end(_array + L) { }
        StaticArray(T const& initValue)
        :   _end(_array + L)
        {
            for(std::size_t ii = 0; ii != L; ++ii) {
                _array[ii] = initValue;
            }
        }
        ~StaticArray() { }

        /* Const */
        T const* begin() const      { return _array; }
        T const* end() const        { return _end; }
        std::size_t length() const  { return L; }
        T const& operator[](std::size_t index) const    
        { 
            assert(index < L);
            return _array[index]; 
        }

        /* Non-Const */
        T* begin()                  { return _array; }
        T* end()                    { return _end; }
        T& operator[](std::size_t index)    
        { 
            assert(index < L);
            return _array[index]; 
        }
    private:
        T _array[L];
        T* _end;
    };

    template <typename T, std::size_t L>
    class StaticArray<T,L,false> {
    public:
        StaticArray() : _arrayPtr(reinterpret_cast<T*>(_array)), _end(_arrayPtr + L) { }
        ~StaticArray() { }

        // Const
        T const* begin() const      { return _arrayPtr; }
        T const* end() const        { return _end; }
        std::size_t length() const  { return L; }
        T const& operator[](std::size_t index) const    
        { 
            assert(index < L);
            return _arrayPtr[index]; 
        }

        // Non-Const
        T* begin()                  { return _arrayPtr; }
        T* end()                    { return _end; }
        T& operator[](std::size_t index)    
        { 
            assert(index < L);
            return _arrayPtr[index]; 
        }
    private:
        char _array[L * sizeof(T)];
        T* _arrayPtr;
        T* _end;
    };
}

/***
 * Implementation
 ***/
template <typename T, bool PADDED, bool CTOR_INIT, bool COPY_SAFE>
promote::Array<T,PADDED,CTOR_INIT,COPY_SAFE>::Array(std::size_t const length)
:   _array(CTOR_INIT ? new Entry[length] : static_cast<Entry*>(::operator new(length * sizeof(Entry)))),
    _end(_array + length),
    _length(length)
{
}

template <typename T, bool PADDED, bool CTOR_INIT, bool COPY_SAFE>
promote::Array<T,PADDED,CTOR_INIT,COPY_SAFE>::~Array() {
    if( CTOR_INIT ) {
        delete [] _array;
    } else {
        ::operator delete(_array);
    }
}

template <typename T, bool PADDED, bool CTOR_INIT, bool COPY_SAFE>
void promote::Array<T,PADDED,CTOR_INIT,COPY_SAFE>::resize(std::size_t const length) {
    if( CTOR_INIT ) {
        Entry* const newArray(new Entry[length]);
        std::size_t const lesserLength(length < _length ? length : _length);
        for(std::size_t ii = 0; ii != lesserLength; ++ii) {
            newArray[ii] = _array[ii];
        }
        delete [] _array;
        _array = newArray;
    } else if( COPY_SAFE ) {
        _array = static_cast<Entry*>(realloc(_array, length * sizeof(Entry)));
    } else {
        Entry* const newArray(static_cast<Entry*>(::operator new(length * sizeof(Entry))));
        std::size_t const lesserLength(length < _length ? length : _length);
        for(std::size_t ii = 0; ii != lesserLength; ++ii) {
            new(&(newArray[ii])) Entry(_array[ii]);
            _array[ii].~Entry();
        }
        for(std::size_t ii = lesserLength; ii < _length; ++ii) {
            _array[ii].~Entry();
        }
        ::operator delete(_array);
        _array = newArray;
    }
    _end = _array + length;
    _length = length;
}

#endif /* PROMOTE_ARRAY_HPP_ */

