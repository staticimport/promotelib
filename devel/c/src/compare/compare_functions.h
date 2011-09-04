#ifndef PROMOTE_COMPARE_FUNCTIONS_H_
#define PROMOTE_COMPARE_FUNCTIONS_H_

#include "comparator.h"

pmt_order_t pmt_compare_char(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_double(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_float(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_int(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_int8(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_int16(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_int32(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_int64(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_long(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_longlong(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_short(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_sizet(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_uchar(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_uint(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_uint8(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_uint16(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_uint32(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_uint64(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_ushort(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_ulong(void* notUsed, pmt_universal_t a, pmt_universal_t b);
pmt_order_t pmt_compare_ulonglong(void* notUsed, pmt_universal_t a, pmt_universal_t b);

// Strings
pmt_order_t pmt_compare_string(void* notUsed, pmt_universal_t a, pmt_universal_t b);

#endif /* PROMOTE_COMPARE_FUNCTIONS_H_ */

