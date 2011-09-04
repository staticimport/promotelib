#ifndef PROMOTE_ITEM_H_
#define PROMOTE_ITEM_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef union pmt_item {
  bool b;
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
} pmt_item_t;

/**
 * Prototypes
 **/

// Init
static inline pmt_item_t pmt_item_bool(bool const b);
static inline pmt_item_t pmt_item_char(char const c);
static inline pmt_item_t pmt_item_double(double const d);
static inline pmt_item_t pmt_item_float(float const f);
static inline pmt_item_t pmt_item_int(int const i);
static inline pmt_item_t pmt_item_int8(int8_t const i8);
static inline pmt_item_t pmt_item_int16(int16_t const i16);
static inline pmt_item_t pmt_item_int32(int32_t const i32);
static inline pmt_item_t pmt_item_int64(int64_t const i64);
static inline pmt_item_t pmt_item_long(long const l);
static inline pmt_item_t pmt_item_longlong(long long const ll);
static inline pmt_item_t pmt_item_short(short const s);
static inline pmt_item_t pmt_item_sizet(size_t const szt);
static inline pmt_item_t pmt_item_uint8(uint8_t const u8);
static inline pmt_item_t pmt_item_uint16(uint16_t const u16);
static inline pmt_item_t pmt_item_uint32(uint32_t const u32);
static inline pmt_item_t pmt_item_uint64(uint64_t const u64);
static inline pmt_item_t pmt_item_uint(unsigned const u);
static inline pmt_item_t pmt_item_uchar(unsigned char const uc);
static inline pmt_item_t pmt_item_ushort(unsigned short const us);
static inline pmt_item_t pmt_item_ulong(unsigned long const ul);
static inline pmt_item_t pmt_item_ulonglong(unsigned long long const ull);
static inline pmt_item_t pmt_item_ptr(void *const ptr);
static inline pmt_item_t pmt_item_constptr(void const *const cptr);

// Equality
static inline bool 
pmt_item_are_equal(pmt_item_t const u1, pmt_item_t const u2);

/**
 * Implementation
 **/
static inline pmt_item_t
pmt_item_bool(bool const b)
{
  pmt_item_t value;
  value.b = b;
  return value;
}

static inline pmt_item_t
pmt_item_char(char const c)
{
  pmt_item_t value;
  value.c = c;
  return value;
}

static inline pmt_item_t
pmt_item_double(double const d)
{
  pmt_item_t value;
  value.d = d;
  return value;
}

static inline pmt_item_t
pmt_item_float(float const f)
{
  pmt_item_t value;
  value.f = f;
  return value;
}

static inline pmt_item_t
pmt_item_int(int const i)
{
  pmt_item_t value;
  value.i = i;
  return value;
}

static inline pmt_item_t
pmt_item_int8(int8_t const i8)
{
  pmt_item_t value;
  value.i8 = i8;
  return value;
}

static inline pmt_item_t
pmt_item_int16(int16_t const i16)
{
  pmt_item_t value;
  value.i16 = i16;
  return value;
}

static inline pmt_item_t
pmt_item_int32(int32_t const i32)
{
  pmt_item_t value;
  value.i32 = i32;
  return value;
}

static inline pmt_item_t
pmt_item_int64(int64_t const i64)
{
  pmt_item_t value;
  value.i64 = i64;
  return value;
}

static inline pmt_item_t
pmt_item_long(long const l)
{
  pmt_item_t value;
  value.l = l;
  return value;
}

static inline pmt_item_t
pmt_item_longlong(long long const ll)
{
  pmt_item_t value;
  value.ll = ll;
  return value;
}

static inline pmt_item_t
pmt_item_short(short const s)
{
  pmt_item_t value;
  value.s = s;
  return value;
}

static inline pmt_item_t
pmt_item_sizet(size_t const szt)
{
  pmt_item_t value;
  value.szt = szt;
  return value;
}

static inline pmt_item_t
pmt_item_uint8(uint8_t const u8)
{
  pmt_item_t value;
  value.u8 = u8;
  return value;
}

static inline pmt_item_t
pmt_item_uint16(uint16_t const u16)
{
  pmt_item_t value;
  value.u16 = u16;
  return value;
}

static inline pmt_item_t
pmt_item_uint32(uint32_t const u32)
{
  pmt_item_t value;
  value.u32 = u32;
  return value;
}

static inline pmt_item_t
pmt_item_uint64(uint64_t const u64)
{
  pmt_item_t value;
  value.u64 = u64;
  return value;
}

static inline pmt_item_t
pmt_item_uint(unsigned const u)
{
  pmt_item_t value;
  value.u = u;
  return value;
}

static inline pmt_item_t
pmt_item_uchar(unsigned char const uc)
{
  pmt_item_t value;
  value.uc = uc;
  return value;
}

static inline pmt_item_t
pmt_item_ushort(unsigned short const us)
{
  pmt_item_t value;
  value.us = us;
  return value;
}

static inline pmt_item_t
pmt_item_ulong(unsigned long const ul)
{
  pmt_item_t value;
  value.ul = ul;
  return value;
}

static inline pmt_item_t
pmt_item_ulonglong(unsigned long long const ull)
{
  pmt_item_t value;
  value.ull = ull;
  return value;
}

static inline pmt_item_t
pmt_item_ptr(void *const ptr)
{
  pmt_item_t value;
  value.ptr = ptr;
  return value;
}

static inline pmt_item_t
pmt_item_constptr(void const *const cptr)
{
  pmt_item_t value;
  value.cptr = cptr;
  return value;
}

static inline bool
pmt_item_are_equal(pmt_item_t const u1, pmt_item_t const u2)
{
  return u1.u64 == u2.u64;
}

#endif /* PROMOTE_ITEM_H_ */

