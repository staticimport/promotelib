#ifndef PROMOTE_HASH_FUNCTIONS_H_
#define PROMOTE_HASH_FUNCTIONS_H_

#include "hash.h"

/**
 * SAX
 **/
pro_hash_t pro_hash_sax(void const* const restrict ptr, size_t const len);
pro_hash_t pro_hash_sax_string(pro_item_t const item);
pro_item_t pro_hash_sax_item(pmt_item_t const uni);

#endif /* PROMOTE_HASH_FUNCTIONS_H_ */

