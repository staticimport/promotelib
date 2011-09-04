#ifndef PROMOTE_HASH_HPP_
#define PROMOTE_HASH_HPP_

#include <climits>
#include <iostream>
#include <stdint.h>

#include "System.hpp"
#include "Utilities.hpp"

namespace promote
{
#ifdef __LP64__
    typedef uint64_t hash_t;
    static const hash_t HASH_MAX = UINT64_MAX;
#else
    typedef uint32_t hash_t;
    static const hash_t HASH_MAX = UINT32_MAX;
#endif
    static const hash_t HASH_MIN = 0;


    template <typename T>
    class Hash
    {
    public:
        virtual ~Hash() { }
        virtual hash_t hash(T const& x) const = 0;
    };
}

#endif /* PROMOTE_HASH_HPP_ */

