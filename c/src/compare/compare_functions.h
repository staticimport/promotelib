#ifndef PROMOTE_COMPARE_FUNCTIONS_H_
#define PROMOTE_COMPARE_FUNCTIONS_H_

#include "comparator.h"

pro_order_t pro_compare_char(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_double(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_float(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_int(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_int8(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_int16(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_int32(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_int64(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_long(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_longlong(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_short(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_sizet(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_uchar(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_uint(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_uint8(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_uint16(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_uint32(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_uint64(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_ushort(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_ulong(void* not_used, pro_item_t const a, pro_item_t const b);
pro_order_t pro_compare_ulonglong(void* not_used, pro_item_t const a, pro_item_t const b);

// Strings
pro_order_t pro_compare_string(void* not_used, pro_item_t const a, pro_item_t const b);

#endif /* PROMOTE_COMPARE_FUNCTIONS_H_ */

