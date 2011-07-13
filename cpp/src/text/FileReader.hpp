#ifndef PROMOTE_FILE_READER_HPP_
#define PROMOTE_FILE_READER_HPP_

#include <exception>

#include "Buffer.hpp"
#include "FormattedString.hpp"
#include "Iterator.hpp"
#include "String.hpp"

namespace promote {
  class FileReaderException : public std::exception {
  public:
    FileReaderException(promote::String const& reason) : _reason(reason) { }
    ~FileReaderException() throw() { }
    char const* what() const throw() { return _reason.c_str(); }
  private:
    promote::String const _reason;
  };

  class FileReader : public Iterator<char> {
  public:
    FileReader(char const* const filename, 
               bool const concurrent = false);
    ~FileReader();

    inline bool hasNext() const;
    inline char next();
  private:
    inline bool eof() const;
    std::size_t readIntoBuffer();

    FILE* const _fp;
    Buffer<char> _buffer;
    bool const _concurrent;
  };
}

/***
 * Implementation
 ***/
inline bool promote::FileReader::hasNext() const
{
  return !_buffer.isEmpty() or !eof();
}

inline char promote::FileReader::next()
{
  if( _buffer.isEmpty() ) readIntoBuffer();
  char const c(*_buffer.head());
  _buffer.add(1,false);
  return c;
}

inline bool promote::FileReader::eof() const
{
  return (_concurrent ? feof(_fp) : feof_unlocked(_fp)) != 0;
}

#endif /* PROMOTE_FILE_READER_HPP_ */

