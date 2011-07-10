#ifndef PROMOTE_FILE_READER_HPP_
#define PROMOTE_FILE_READER_HPP_

#include <exception>

#include "FormattedString.hpp"
#include "Iterator.hpp"
#include "String.hpp"

namespace promote {
    class String;

    class FileReaderException : public std::exception {
    public:
        FileReaderException(promote::String const& reason) : _reason(reason) { }
        ~FileReaderException() throw() { }
        char const* what() const throw() { return _reason.c_str(); }
    private:
        promote::String const _reason;
    };

    template <bool CONCURRENT=false>
    class FileReader : public Iterator<char> {
    public:
        FileReader(String const& filename);
        ~FileReader();

        // Const
        bool hasNext() const {
            return !_buffer.isEmpty() or !feof_unlocked(_fp);
        }
        char next() {
            if( _buffer.isEmpty() ) readIntoBuffer();
            char const c(*_buffer.head());
            _buffer.add(1,false);
            return c;
        }
    private:
        void readIntoBuffer();

        FILE* const _fp;
        Buffer<char> _buffer;
    };
}

template <bool CONCURRENT>
promote::FileReader::FileReader(char const* const filename)
:   _fp(fopen(filename)),
    _buffer(1024, 2.0)
{
    if( _fp == NULL ) {
        promote::FormattedString const reason("Unable to open file: %s", filename);
        throw FileReaderException(reason);
    }
}

template <bool CONCURRENT>
promote::FileReader::~FileReader()
{
    fclose(_fp);
}

template <bool CONCURRENT>
bool promote::FileReader::readIntoBuffer()
{
    std::size_t const countRead(fread_unlocked(_buffer.head(), 1, 
                                               _buffer.available(), _fp));
    if( countRead ) {
        return countRead;
    } else {
        if( feof_unlocked(_fp) ) {
            throw FileReaderException("End of file reached.");
        } else if( ferror_unlocked(_fp) ) {
            throw FileReaderException("Error encountered.");
        }
    }
}

#endif /* PROMOTE_FILE_READER_HPP_ */

