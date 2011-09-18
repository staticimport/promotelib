
#include "Exception.hpp"

using namespace promote;

Exception::Exception(char const* const reason)
: _reason(reason)
{
}

Exception::Exception(std::string const& reason)
: _reason(reason)
{
}

Exception::~Exception() throw()
{
}

