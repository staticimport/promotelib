
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "compare_functions.h"

#define PRO_CAST_COMPARE(type, a, b) \
        type const typeA = *(type*)(a); \
        type const typeB = *(type*)(b); \
        return (typeA < typeB) ? PRO_ORDER_INCREASING : \
               (typeA != typeB ? PRO_ORDER_SAME : PRO_ORDER_DECREASING);


#define PRO_COMPARE(a,b) \
        return (a) < (b) ? PRO_ORDER_INCREASING : \
               ((a) == (b) ? PRO_ORDER_SAME : PRO_ORDER_DECREASING); 


pro_order_t pro_compare_char(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.c, b.c)
}
pro_order_t pro_compare_double(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.d, b.d)
}
pro_order_t pro_compare_float(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.f, b.f)
}
pro_order_t pro_compare_int(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.i, b.i)
}
pro_order_t pro_compare_int8(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.i8, b.i8)
}
pro_order_t pro_compare_int16(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.i16, b.i16)
}
pro_order_t pro_compare_int32(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.i32, b.i32)
}
pro_order_t pro_compare_int64(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.i64, b.i64)
}
pro_order_t pro_compare_long(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.l, b.l)
}
pro_order_t pro_compare_longlong(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.ll, b.ll)
}
pro_order_t pro_compare_short(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.s, b.s)
}
pro_order_t pro_compare_sizet(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.szt, b.szt)
}
pro_order_t pro_compare_uchar(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.uc, b.uc)
}
pro_order_t pro_compare_uint(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.u, b.u)
}
pro_order_t pro_compare_uint8(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.u8, b.u8)
}
pro_order_t pro_compare_uint16(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.u16, b.u16)
}
pro_order_t pro_compare_uint32(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.u32, b.u32)
}
pro_order_t pro_compare_uint64(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.u64, b.u64)
}
pro_order_t pro_compare_ushort(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.us, b.us)
}
pro_order_t pro_compare_ulong(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.ul, b.ul)
}
pro_order_t pro_compare_ulonglong(void* not_used, pro_item_t const a, pro_item_t const b)
{
    PRO_COMPARE(a.ull, b.ull)
}

// String
pro_order_t pro_compare_string(void* not_used, pro_item_t const a, pro_item_t const b)
{
    int const res = strcmp((char const*)a.cptr, (char const*)b.cptr);
    return res < 0 ? PRO_ORDER_INCREASING : 
           (res == 0 ? PRO_ORDER_SAME : PRO_ORDER_DECREASING);
}

