#ifndef PROMOTE_FORMATTED_STRING_HPP_
#define PROMOTE_FORMATTED_STRING_HPP_

#include "String.hpp"

namespace promote
{
    class FormattedString : public String
    {
    public:
        FormattedString(char const* format, ...);
    };
}

#endif /* PROMOTE_FORMATTED_STRING_HPP_ */

