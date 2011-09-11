#ifndef PROMOTE_RANDOM_FUNCTIONS_HPP
#define PROMOTE_RANDOM_FUNCTIONS_HPP

#include "random_generator.h"

/**
 * Prototypes
 **/

// Standard
static inline pro_rand_t 
pro_random_standard(void* not_used);

static pro_randgen_t const PRO_STANDARD_RANDGEN = {
  &pro_random_standard, 0
};

/**
 * Inline Implementations
 **/
static inline pro_rand_t
pro_random_standard(void* not_used)
{
  return (pro_rand_t)random();
}

#endif /* PROMOTE_RANDOM_FUNCTIONS_HPP_ */

