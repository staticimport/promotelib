#ifndef PROMOTE_RANDOM_GENERATOR_H_
#define PROMOTE_RANDOM_GENERATOR_H_

#include <stdbool.h>
#include <stdlib.h>

typedef long pmt_rand_t;

typedef struct pmt_randgen {
    pmt_rand_t (*gen)(void* arg0);
    void* arg0;
} pmt_randgen_t;

bool pmt_randgen_init(pmt_randgen_t* const restrict randgen,
                      pmt_rand_t (*gen)(void* arg0),
                      void* arg0);
bool pmt_randgen_init_copy(pmt_randgen_t* const restrict dest,
                           pmt_randgen_t* const restrict source);
                            

static inline pmt_rand_t
pmt_randgen_generate(pmt_randgen_t* const restrict randgen)
{
    if( randgen->gen ) {
        return (*(randgen->gen))(randgen->arg0);
    } else {
#ifdef x86_64
        return ((pmt_rand_t)random()) << 32 | (pmt_rand_t)random();
#else
        return (pmt_rand_t)random();
#endif
    }
}

#endif /* PROMOTE_RANDOM_GENERATOR_H_ */

