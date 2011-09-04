
#include <cstdarg>
#include <cstdio>

#include "FormattedString.hpp"

promote::FormattedString::FormattedString(char const* format, ...)
:   promote::String("",0,false)
{
    va_list va;
    char buffer[2048];
    char* bufferPtr(0);

    va_start(va, format);
    int const length = vsnprintf(buffer, 2048, format, va);
    va_end(va);
    if( length < 2048 ) {
        bufferPtr = buffer;
    } else {
        bufferPtr = new char[length+1];
        va_start(va, format);
        vsprintf(bufferPtr, format, va);
        va_end(va);
    }

    set(bufferPtr, static_cast<promote::String::size_t>(length), true); 
    if( bufferPtr != buffer ) {
        delete [] bufferPtr;
    }
}

