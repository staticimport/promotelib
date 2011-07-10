#ifndef PROMOTE_UNIVERSAL_H_
#define PROMOTE_UNIVERSAL_H_

#include <stddef.h>
#include <stdint.h>

typedef union pmt_universal {
    char c;
    double d;
    float f;
    int i;
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    long l;
    long long ll;
    short s;
    size_t szt;
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    unsigned u;
    unsigned char uc;
    unsigned short us;
    unsigned long ul;
    unsigned long long ull;
    void* ptr;
    void const* cptr;
} pmt_universal_t;

static inline pmt_universal_t
pmt_universal_char(char const c)
{
    pmt_universal_t value;
    value.c = c;
    return value;
}

static inline pmt_universal_t
pmt_universal_double(double const d)
{
    pmt_universal_t value;
    value.d = d;
    return value;
}

static inline pmt_universal_t
pmt_universal_float(float const f)
{
    pmt_universal_t value;
    value.f = f;
    return value;
}

static inline pmt_universal_t
pmt_universal_int(int const i)
{
    pmt_universal_t value;
    value.i = i;
    return value;
}

static inline pmt_universal_t
pmt_universal_int8(int8_t const i8)
{
    pmt_universal_t value;
    value.i8 = i8;
    return value;
}

static inline pmt_universal_t
pmt_universal_int16(int16_t const i16)
{
    pmt_universal_t value;
    value.i16 = i16;
    return value;
}

static inline pmt_universal_t
pmt_universal_int32(int32_t const i32)
{
    pmt_universal_t value;
    value.i32 = i32;
    return value;
}

static inline pmt_universal_t
pmt_universal_int64(int64_t const i64)
{
    pmt_universal_t value;
    value.i64 = i64;
    return value;
}

static inline pmt_universal_t
pmt_universal_long(long const l)
{
    pmt_universal_t value;
    value.l = l;
    return value;
}

static inline pmt_universal_t
pmt_universal_longlong(long long const ll)
{
    pmt_universal_t value;
    value.ll = ll;
    return value;
}

static inline pmt_universal_t
pmt_universal_short(short const s)
{
    pmt_universal_t value;
    value.s = s;
    return value;
}

static inline pmt_universal_t
pmt_universal_sizet(size_t const szt)
{
    pmt_universal_t value;
    value.szt = szt;
    return value;
}

static inline pmt_universal_t
pmt_universal_uint8(uint8_t const u8)
{
    pmt_universal_t value;
    value.u8 = u8;
    return value;
}

static inline pmt_universal_t
pmt_universal_uint16(uint16_t const u16)
{
    pmt_universal_t value;
    value.u16 = u16;
    return value;
}

static inline pmt_universal_t
pmt_universal_uint32(uint32_t const u32)
{
    pmt_universal_t value;
    value.u32 = u32;
    return value;
}

static inline pmt_universal_t
pmt_universal_uint64(uint64_t const u64)
{
    pmt_universal_t value;
    value.u64 = u64;
    return value;
}

static inline pmt_universal_t
pmt_universal_uint(unsigned const u)
{
    pmt_universal_t value;
    value.u = u;
    return value;
}

static inline pmt_universal_t
pmt_universal_uchar(unsigned char const uc)
{
    pmt_universal_t value;
    value.uc = uc;
    return value;
}

static inline pmt_universal_t
pmt_universal_ushort(unsigned short const us)
{
    pmt_universal_t value;
    value.us = us;
    return value;
}

static inline pmt_universal_t
pmt_universal_ulong(unsigned long const ul)
{
    pmt_universal_t value;
    value.ul = ul;
    return value;
}

static inline pmt_universal_t
pmt_universal_ulonglong(unsigned long long const ull)
{
    pmt_universal_t value;
    value.ull = ull;
    return value;
}

static inline pmt_universal_t
pmt_universal_ptr(void *const ptr)
{
    pmt_universal_t value;
    value.ptr = ptr;
    return value;
}

static inline pmt_universal_t
pmt_universal_constptr(void const *const cptr)
{
    pmt_universal_t value;
    value.cptr = cptr;
    return value;
}


#endif /* PROMOTE_UNIVERSAL_H_ */

