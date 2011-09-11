#ifndef PROMOTE_HASH_FUNCTIONS_H_
#define PROMOTE_HASH_FUNCTIONS_H_

#include "hash.h"

/**
 * SAX
 **/
pro_hash_t pro_hash_sax(void const* const restrict ptr, size_t const len);
pro_hash_t pro_hash_sax_string(void* not_used, pro_item_t const item);
pro_hash_t pro_hash_sax_item(void* not_used, pro_item_t const item);
static pro_hash_functor_t const PRO_SAX_HASH_FUNCTOR = {
  &pro_hash_sax_item, 0, 0 
};
static pro_hash_functor_t const PRO_SAX_STRING_HASH_FUNCTOR = {
  &pro_hash_sax_string, 0, 0
};


#endif /* PROMOTE_HASH_FUNCTIONS_H_ */

