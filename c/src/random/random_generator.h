#ifndef PROMOTE_RANDOM_GENERATOR_H_
#define PROMOTE_RANDOM_GENERATOR_H_

#include <stdbool.h>
#include <stdlib.h>

typedef int32_t pro_rand_t;

typedef struct pro_randgen {
    pro_rand_t (*gen)(void* arg0);
    void* arg0;
} pro_randgen_t;

/**
 * Prototypes
 **/
bool pro_randgen_init(pro_randgen_t* const restrict randgen,
                      pro_rand_t (*gen)(void* arg0),
                      void* arg0);
bool pro_randgen_init_copy(pro_randgen_t* const restrict dest,
                           pro_randgen_t* const restrict source);
static inline pro_rand_t
pro_randgen_generate(pro_randgen_t* const restrict randgen);
                            

/**
 * Inline Implementations
 **/
static inline pro_rand_t
pro_randgen_generate(pro_randgen_t* const restrict randgen)
{
  return (*(randgen->gen))(randgen->arg0);
}

#endif /* PROMOTE_RANDOM_GENERATOR_H_ */

