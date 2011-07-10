#ifndef PROMOTE_STANDARD_RANDOM_GENERATOR_HPP_
#define PROMOTE_STANDARD_RANDOM_GENERATOR_HPP_

#include "RandomGenerator.hpp"

namespace promote
{
    class StandardRandomGenerator : public RandomGenerator
    {
    public:
        StandardRandomGenerator()  { }
        ~StandardRandomGenerator() { }

        rand_t generate() const;

        static const StandardRandomGenerator& instance()
        {
            static StandardRandomGenerator instance;
            return instance;
        }
    };
}

#endif /* PROMOTE_STANDARD_RANDOM_GENERATOR_HPP_ */

