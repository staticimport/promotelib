#ifndef PROMOTE_STACK_HPP
#define PROMOTE_STACK_HPP

#include <cstdlib>
#include <new>

#include "Array.hpp"
#include "Exception.hpp"
#include "Padded.hpp"
#include "Structors.hpp"
#include "Utilities.hpp"

namespace promote
{
    namespace stack
    {
        template <typename T, bool COPY=IsClass<T>::No and IsReference<T>::No>
        struct Traits
        {
            static T* allocate(size_t const capacity)
            {
                std::cout << "Here\n";
                return ::new T[capacity];
            }

            static void deallocate(T* const begin, T* const end)
            {
                for(T* const entry = begin; entry != end; ++entry) {
                    entry.~T();
                }
                ::delete [] begin;
            }
            
            static T* expand(T* const array, size_t const oldCapacity, size_t const newCapacity)
            {
                T* const newArray(::new T[newCapacity]);
                for(size_t ii = 0; ii != oldCapacity; ++ii) {
                    new(&(newArray[ii])) T(array[ii]);
                    array[ii].~T();
                }
                ::delete [] array;
                return newArray;
            }

            static void pop(T& element)
            {
                element.~T();
            }

            static void set(T& dest, T const& source)
            {
                new(&dest) T(source);
            }
        };

        template <typename T>
        struct Traits<T,true>
        {
            static T* allocate(size_t const capacity)
            {
                return static_cast<T*>(malloc(capacity * sizeof(T)));
            }

            static void deallocate(T* const begin, T* const end)
            {
                free(begin);
            }

            static T* expand(T* const array, size_t const oldCapacity, size_t const newCapacity)
            {
                return static_cast<T*>(realloc(array, newCapacity * sizeof(T)));
            }

            static void pop(T element) { }

            static void set(T& dest, T const source)
            {
                dest = source;
            }
        };
    }

    template <typename T, typename STRUCTORS=AutoStructors<T> >
    class Stack {
    private:
        typedef Array<T,false,false> StackArray;

        template <typename S, typename V>
        class StackIterator {
        public:
            struct Exception : public promote::Exception {
                Exception(String const& reason) : promote::Exception(reason) { }
            };

            StackIterator(S& stack);
            StackIterator(StackIterator const& iter);
            
            bool hasNext() const { return _next != _end; }
            V& next() {
                if( hasNext() ) {
                    return _next--;
                } else {
                    throw Exception("Iterated beyond bottom of stack.");
                }
            }
        private:
            V* _next;
            V* _end;
        };
    public:
        typedef StackIterator<Stack const, T const> ConstIterator;
        typedef StackIterator<Stack, T> Iterator;

        Stack(size_t const initCapacity=16);
        ~Stack();

        // Const
        bool empty() const                              { return _array.begin() == _top; }
        typename Passable<T const>::Type peek() const   { return _array.value(*_top); }
        size_t size() const                             { return _top - _array.begin(); }

        // Non-Const
        typename Passable<T>::Type peek()               { return _array.value(*_top); }
        void pop()                                      { STRUCTORS::destruct(_array.value(*(_top--))); }
        void push(typename Passable<T const>::Type x) 
        {
            if( _top == _array.end() ) expand();
            STRUCTORS::construct(_array.value(*(_top++)), x);
        }
    private:
        void expand()
        {
            std::size_t const oldLength(_array.length());
            _array.reserve(oldLength << 1);
            _top = _array.begin() + oldLength;
        }

        StackArray _array;
        typename StackArray::Entry* _top;
    };
}

template <typename T, typename STRUCTORS>
promote::Stack<T,STRUCTORS>::Stack(size_t const initCapacity)
:   _array(initCapacity),
    _top(_array.begin())
{
}

template <typename T, typename STRUCTORS>
promote::Stack<T,STRUCTORS>::~Stack()
{
    while( !empty() ) {
        pop();
    }
}

    /*template <typename T, bool COPY_SAFE=IsClass<T>::No>
    class Stack
    {
    public:
        Stack(size_t const initCapacity=16);
        ~Stack();

        // Const
        bool empty() const      { return _size == 0; }
        size_t size() const     { return _size; }
        T const& top() const    { return _array[_size]; }

        // Non-Const
        void push(T const& x)
        {
            if( _size == _capacity ) expand();
            new(&(_array[_size++])) T(x);
        }
        void pop()              { _array[_size--].~T(); }
        T& top()                { return _array[_size]; }
    private:
        void expand()
        {
            size_t const newCapacity(_capacity << 1);
            T* const newArray(::new T[newCapacity]);
            for(size_t ii = 0; ii != _capacity; ++ii) {
                new(&(newArray[ii])) T(_array[ii]);
                _array[ii].~T();
            }
            ::delete [] _array;
            _array = newArray;
            _capacity = newCapacity;
        }

        T* _array;
        size_t _capacity;
        size_t _size;
    };

    template <typename T>
    class Stack<T,true>
    {
    public:
        Stack(size_t const initCapacity=16);
        ~Stack();

        // Const
        bool empty() const      { return _size == 0; }
        size_t size() const     { return _size; }
        T const& top() const    { return _array[_size]; }

        // Non-Const
        void push(T const& x)
        {
            if( _size == _capacity ) expand();
            _array[_size++] = x;
        }
        void pop()              { --_size; }
        T& top()                { return _array[_size]; }
    private:
        void expand()
        {
            _capacity <<= 1;
            _array = static_cast<T*>(realloc(_array, _capacity * sizeof(T)));
        }

        T* _array;
        size_t _capacity;
        size_t _size;
    };
}

// Implementation
template <typename T, bool COPY_SAFE>
promote::Stack<T,COPY_SAFE>::Stack(size_t const initCapacity)
:   _array(::new T[initCapacity]),
    _capacity(initCapacity),
    _size(0)
{
}

template <typename T>
promote::Stack<T,true>::Stack(size_t const initCapacity)
:   _array(static_cast<T*>(malloc(initCapacity * sizeof(T)))),
    _capacity(initCapacity),
    _size(0)
{
}

template <typename T, bool COPY_SAFE>
promote::Stack<T,COPY_SAFE>::~Stack()
{
    for(size_t ii = 0; ii != _size; ++ii) {
        _array[ii].~T();
    }
    ::delete [] _array;
}

template <typename T>
promote::Stack<T,true>::~Stack()
{
    free(_array);
}*/

#endif /* PROMOTE_STACK_HPP */

