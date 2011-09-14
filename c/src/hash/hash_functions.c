
#include "hash_functions.h"

#define PROMOTE_HASH_SAX(ptr, len) \
  { \
    char const* p = (char const*)ptr; \
    size_t const l = len; \
    pro_hash_t value = 0; \
    for(size_t ii = 0; ii != l; ++ii) { \
      value ^= (value << 5) + (value >> 2) + p[ii]; \
    } \
    return value; \
  }

pro_hash_t pro_hash_sax(void const* const restrict ptr, size_t const len)
{
  PROMOTE_HASH_SAX(ptr, len)
}

pro_hash_t pro_hash_sax_string(void* not_used, pro_item_t const item)
{
  char const* cptr = (char const*)item.cptr;
  char c;
  pro_hash_t value = 0;
  while ((c = *(cptr++))) {
    value ^= (value << 5) + (value >> 2) + c;
  }
  return value;
}

pro_hash_t pro_hash_sax_item(void* not_used, pro_item_t const item)
{
  PROMOTE_HASH_SAX(&item, sizeof(pro_item_t))
}
