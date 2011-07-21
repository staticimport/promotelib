
#include "FileReader.hpp"

using namespace promote;

FileReader::FileReader(char const* const filename, 
                       bool const concurrent)
: _fp(fopen(filename, "r")),
  _buffer(1024, 2.0),
  _concurrent(concurrent)
{
  if( _fp == NULL ) {
    promote::FormattedString const reason("Unable to open file: %s", filename);
    throw FileReaderException(reason);
  }
}

FileReader::~FileReader()
{
  fclose(_fp);
}

bool FileReader::hasNext()
{
  return !_buffer.isEmpty() or !eof();
}

char promote::FileReader::next()
{
  if( _buffer.isEmpty() ) readIntoBuffer();
  char const c(*_buffer.head());
  _buffer.add(1,false);
  return c;
}

std::size_t FileReader::readIntoBuffer()
{
  //std::size_t const countRead = _concurrent ?
  //  fread(_buffer.head(), 1, _buffer.available(), _fp) :
  //  fread_unlocked(_buffer.head(), 1, _buffer.available(), _fp);
  std::size_t const countRead(fread(_buffer.head(), 1, _buffer.available(), _fp));
  if( countRead ) {
    return countRead;
  } else {
    if( eof() ) {
      throw FileReaderException("End of file reached.");
    } else if( _concurrent ? ferror(_fp) : ferror_unlocked(_fp) ) {
      throw FileReaderException("Error encountered.");
    } else {
      throw FileReaderException("Unknown problem reading from file.");
    }
  }
}
