#ifndef PROMOTE_BIT_VECTOR_H_
#define PROMOTE_BIT_VECTOR_H_

#include <stdbool.h>
#include <stddef.h>

#include "system_utils.h"

#define PMT_BITVEC_IN_WORD_INDEX(index) (PMT_WORD_MASK & (index))
#define PMT_BITVEC_WORD_INDEX(index) ((index) >> PMT_WORD_BYTES)

typedef struct pmt_bit_vector {
    pmt_uword_t* data;
    size_t data_length;
    size_t num_bits_set;
} pmt_bit_vector_t;

/* Init / Unit */
bool pmt_bitvec_init(pmt_bit_vector_t *const restrict bv,
                     size_t const init_capacity);
bool pmt_bitvec_uninit(pmt_bit_vector_t* const restrict bv);

/* Observation */
static inline
bool pmt_bitvec_is_set(pmt_bit_vector_t const *const restrict bv,
                       size_t const index)
{
    size_t const word_index = PMT_BITVEC_WORD_INDEX(index);
    size_t const in_word_index = PMT_BITVEC_IN_WORD_INDEX(index);
    return word_index < bv->data_length ?
        bv->data[word_index] >>  in_word_index : false;
}

static inline
pmt_uword_t pmt_bitvec_word(pmt_bit_vector_t const *const restrict bv,
                            size_t const word_index)
{
    return word_index < bv->data_length ? bv->data[word_index] : 0;
}

static inline
void pmt_bitvec_clear(pmt_bit_vector_t *const restrict bv,
                      size_t const index)
{
    size_t const word_index = PMT_BITVEC_WORD_INDEX(index);
    if( word_index < bv->data_length ) {
        bv->data[word_index] &= 
            (PMT_UWORD_MAX - (1 << PMT_BITVEC_IN_WORD_INDEX(index)));
    }
}

bool pmt_bitvec_expand(pmt_bit_vector_t *const restrict bv,
                       size_t const min_capacity);

static inline
bool pmt_bitvec_set(pmt_bit_vector_t *const restrict bv,
                    size_t const index)
{
    size_t const word_index = PMT_BITVEC_WORD_INDEX(index);
    size_t const in_word_index = PMT_BITVEC_IN_WORD_INDEX(index);
    if( word_index < bv->data_length || pmt_bitvec_expand(bv, index+1) ) {
        bv->data[word_index] |= 1 << in_word_index;
        return true;
    } else {
        return false;
    }
}



#endif /* PROMOTE_BIT_VECTOR_H_ */

