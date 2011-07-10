#ifndef PROMOTE_HASH_FUNCTIONS_H_
#define PROMOTE_HASH_FUNCTIONS_H_

#include "hash.h"

/**
 * SAX
 **/
static inline pmt_hash_t
hash_sax(void const* const restrict ptr, size_t const len)
{
    char const* const restrict char_ptr = ptr;
    pmt_hash_t hash = 0;
    for(size_t ii = 0; ii != len; ++ii) {
        hash ^= (hash << 5) + (hash >> 2) + char_ptr[ii];
    }
    return hash;
}

static inline pmt_hash_t
hash_sax_string(pmt_universal_t const uni)
{
    char const* const restrict char_ptr = uni.ptr;
    pmt_hash_t hash = 0;
    for(size_t ii = 0; char_ptr[ii]; ++ii) {
        hash ^= (hash << 5) + (hash >> 2) + char_ptr[ii];
    }
    return hash;
}

static inline pmt_hash_t
hash_sax_universal(pmt_universal_t const uni)
{
    return hash_sax(&(uni.u64), sizeof(uint64_t));
}

#endif /* PROMOTE_HASH_FUNCTIONS_H_ */

