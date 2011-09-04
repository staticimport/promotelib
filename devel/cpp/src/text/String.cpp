
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "SAXHash.hpp"
#include "String.hpp"

promote::String::String()
:   _data(""),
    _length(0),
    _hashcode(0)
{
}

promote::String::String(char const* data,
                        promote::String::size_t const length,
                        bool const copy)
:   _data(data),
    _length(length),
    _hashcode(hashSAX(data, length))
{
    if( copy ) {
        setDeepCopy();
    }
}

promote::String::String(char const* data,
                        bool const copy)
:   _data(data),
    _length(strlen(data)),
    _hashcode(hashSAX(data, _length))
{
    if( copy ) {
        setDeepCopy();
    }
}

promote::String::String(promote::String const& string)
:   _data(string.c_str()),
    _length(string.length()),
    _hashcode(string.hashcode())
{
    setDeepCopy();
}

promote::String::~String()
{
    if( _length & FREE_FLAG ) {
        free(const_cast<char*>(_data));
    }
}

void
promote::String::set(const char* data,
                     const promote::String::size_t length,
                     bool copy)
{
    // TODO: optimize: don't free then malloc if there is space available

    if( _length & FREE_FLAG ) {
        free(const_cast<char*>(_data));
    }

    _data = data;
    _length = length;
    _hashcode = hashSAX(data, length);

    if( copy ) {
        setDeepCopy();
    }
}

void
promote::String::setDeepCopy()
{
    if( _length & FREE_FLAG ) return;

    char* copy(static_cast<char*>(malloc(_length+1)));
    memcpy(copy, _data, _length);
    copy[_length] = 0;
    _data = copy;
    _length |= FREE_FLAG;
}

void 
promote::String::trim()
{
    std::size_t const oldLength(length());
    if( oldLength == 0 ) return;

    // Beginning whitespace
    std::size_t newFirst(0);
    while( newFirst != oldLength and isspace(_data[newFirst]) ) {
        ++newFirst;
    }

    // End whitespace
    std::size_t newLast(oldLength-1);
    while( newLast > newFirst and isspace(_data[newLast]) ) {
        --newLast;
    }

    if( newFirst ) {
        if( _length | FREE_FLAG ) {
            _length = FREE_FLAG | (newLast - newFirst + 1);
            memmove(const_cast<char*>(_data), _data + newFirst, length());
        } else {
            _length = newLast - newFirst + 1;
            _data += newFirst;
        }
    } else if( newLast != oldLength - 1 ) {
        if( _length | FREE_FLAG ) {
            _length = FREE_FLAG | (newLast + 1);
        } else {
            _length = (newLast + 1);
        }
    }
}

