#ifndef PROMOTE_RANDOM_GENERATOR_HPP_
#define PROMOTE_RANDOM_GENERATOR_HPP_

#include <stdint.h>

#include "System.hpp"

namespace promote
{
#ifdef __LP64__
    typedef uint64_t rand_t;
    static const rand_t RANDOM_MAX = UINT64_MAX;
#else
    typedef uint32_t rand_t;
    static const rand_t RANDOM_MAX = UINT32_MAX;
#endif
    static const rand_t RANDOM_MIN = 0;

    class RandomGenerator
    {
    public:
        virtual ~RandomGenerator() { }
        virtual rand_t generate() const = 0;
    };
}

#endif /* PROMOTE_RANDOM_GENERATOR_HPP_ */
