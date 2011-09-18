#ifndef PROMOTE_EXCEPTION_HPP
#define PROMOTE_EXCEPTION_HPP

#include <exception>
#include <string>

namespace promote {
  class Exception : public std::exception {
  public:
    Exception(char const* const reason);
    Exception(std::string const& reason);
    virtual ~Exception() throw ();

    inline char const* what() const throw();
  private:
    std::string const _reason;
  };
}

/**
 * Inline Implementation
 **/
inline char const*
promote::Exception::what() const throw()
{
  return _reason.c_str();
}

#endif /* PROMOTE_EXCEPTION_HPP */

