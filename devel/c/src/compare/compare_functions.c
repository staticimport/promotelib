
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "compare_functions.h"

#define PMT_CAST_COMPARE(type, a, b) \
        type const typeA = *(type*)(a); \
        type const typeB = *(type*)(b); \
        return (typeA < typeB) ? PMT_ORDER_INCREASING : \
               (typeA != typeB ? PMT_ORDER_SAME : PMT_ORDER_DECREASING);


#define PMT_COMPARE(a,b) \
        return (a) < (b) ? PMT_ORDER_INCREASING : \
               ((a) == (b) ? PMT_ORDER_SAME : PMT_ORDER_DECREASING); 


pmt_order_t pmt_compare_char(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.c, b.c)
}
pmt_order_t pmt_compare_double(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.d, b.d)
}
pmt_order_t pmt_compare_float(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.f, b.f)
}
pmt_order_t pmt_compare_int(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.i, b.i)
}
pmt_order_t pmt_compare_int8(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.i8, b.i8)
}
pmt_order_t pmt_compare_int16(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.i16, b.i16)
}
pmt_order_t pmt_compare_int32(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.i32, b.i32)
}
pmt_order_t pmt_compare_int64(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.i64, b.i64)
}
pmt_order_t pmt_compare_long(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.l, b.l)
}
pmt_order_t pmt_compare_longlong(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.ll, b.ll)
}
pmt_order_t pmt_compare_short(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.s, b.s)
}
pmt_order_t pmt_compare_sizet(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.szt, b.szt)
}
pmt_order_t pmt_compare_uchar(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.uc, b.uc)
}
pmt_order_t pmt_compare_uint(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.u, b.u)
}
pmt_order_t pmt_compare_uint8(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.u8, b.u8)
}
pmt_order_t pmt_compare_uint16(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.u16, b.u16)
}
pmt_order_t pmt_compare_uint32(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.u32, b.u32)
}
pmt_order_t pmt_compare_uint64(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.u64, b.u64)
}
pmt_order_t pmt_compare_ushort(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.us, b.us)
}
pmt_order_t pmt_compare_ulong(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.ul, b.ul)
}
pmt_order_t pmt_compare_ulonglong(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    PMT_COMPARE(a.ull, b.ull)
}

// String
pmt_order_t pmt_compare_string(void* notUsed, pmt_universal_t a, pmt_universal_t b)
{
    int const res = strcmp((char const*)a.ptr, (char const*)b.ptr);
    return res < 0 ? PMT_ORDER_INCREASING : 
           (res == 0 ? PMT_ORDER_SAME : PMT_ORDER_DECREASING);
}

