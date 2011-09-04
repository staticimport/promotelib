#ifndef PROMOTE_EXCEPTION_HPP_
#define PROMOTE_EXCEPTION_HPP_

#include <exception>

#include "String.hpp"

namespace promote
{
    class Exception : public std::exception {
    public:
        Exception(String const& reason) : _reason(reason) { }
        virtual ~Exception() throw () { }

        char const* what() const throw() { return _reason.c_str(); }
    private:
        String const& _reason;
    };
}

#endif /* PROMOTE_EXCEPTION_HPP_ */

