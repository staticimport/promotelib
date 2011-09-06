#ifndef PROMOTE_ITEM_H_
#define PROMOTE_ITEM_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef union pro_item {
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
} pro_item_t;

/**
 * Prototypes
 **/

// Init
static inline pro_item_t pro_item_bool(bool const b);
static inline pro_item_t pro_item_char(char const c);
static inline pro_item_t pro_item_double(double const d);
static inline pro_item_t pro_item_float(float const f);
static inline pro_item_t pro_item_int(int const i);
static inline pro_item_t pro_item_int8(int8_t const i8);
static inline pro_item_t pro_item_int16(int16_t const i16);
static inline pro_item_t pro_item_int32(int32_t const i32);
static inline pro_item_t pro_item_int64(int64_t const i64);
static inline pro_item_t pro_item_long(long const l);
static inline pro_item_t pro_item_longlong(long long const ll);
static inline pro_item_t pro_item_short(short const s);
static inline pro_item_t pro_item_sizet(size_t const szt);
static inline pro_item_t pro_item_uint8(uint8_t const u8);
static inline pro_item_t pro_item_uint16(uint16_t const u16);
static inline pro_item_t pro_item_uint32(uint32_t const u32);
static inline pro_item_t pro_item_uint64(uint64_t const u64);
static inline pro_item_t pro_item_uint(unsigned const u);
static inline pro_item_t pro_item_uchar(unsigned char const uc);
static inline pro_item_t pro_item_ushort(unsigned short const us);
static inline pro_item_t pro_item_ulong(unsigned long const ul);
static inline pro_item_t pro_item_ulonglong(unsigned long long const ull);
static inline pro_item_t pro_item_ptr(void *const ptr);
static inline pro_item_t pro_item_constptr(void const *const cptr);

// Equality
static inline bool 
pro_item_are_equal(pro_item_t const u1, pro_item_t const u2);

/**
 * Implementation
 **/
static inline pro_item_t
pro_item_bool(bool const b)
{
  pro_item_t value;
  value.b = b;
  return value;
}

static inline pro_item_t
pro_item_char(char const c)
{
  pro_item_t value;
  value.c = c;
  return value;
}

static inline pro_item_t
pro_item_double(double const d)
{
  pro_item_t value;
  value.d = d;
  return value;
}

static inline pro_item_t
pro_item_float(float const f)
{
  pro_item_t value;
  value.f = f;
  return value;
}

static inline pro_item_t
pro_item_int(int const i)
{
  pro_item_t value;
  value.i = i;
  return value;
}

static inline pro_item_t
pro_item_int8(int8_t const i8)
{
  pro_item_t value;
  value.i8 = i8;
  return value;
}

static inline pro_item_t
pro_item_int16(int16_t const i16)
{
  pro_item_t value;
  value.i16 = i16;
  return value;
}

static inline pro_item_t
pro_item_int32(int32_t const i32)
{
  pro_item_t value;
  value.i32 = i32;
  return value;
}

static inline pro_item_t
pro_item_int64(int64_t const i64)
{
  pro_item_t value;
  value.i64 = i64;
  return value;
}

static inline pro_item_t
pro_item_long(long const l)
{
  pro_item_t value;
  value.l = l;
  return value;
}

static inline pro_item_t
pro_item_longlong(long long const ll)
{
  pro_item_t value;
  value.ll = ll;
  return value;
}

static inline pro_item_t
pro_item_short(short const s)
{
  pro_item_t value;
  value.s = s;
  return value;
}

static inline pro_item_t
pro_item_sizet(size_t const szt)
{
  pro_item_t value;
  value.szt = szt;
  return value;
}

static inline pro_item_t
pro_item_uint8(uint8_t const u8)
{
  pro_item_t value;
  value.u8 = u8;
  return value;
}

static inline pro_item_t
pro_item_uint16(uint16_t const u16)
{
  pro_item_t value;
  value.u16 = u16;
  return value;
}

static inline pro_item_t
pro_item_uint32(uint32_t const u32)
{
  pro_item_t value;
  value.u32 = u32;
  return value;
}

static inline pro_item_t
pro_item_uint64(uint64_t const u64)
{
  pro_item_t value;
  value.u64 = u64;
  return value;
}

static inline pro_item_t
pro_item_uint(unsigned const u)
{
  pro_item_t value;
  value.u = u;
  return value;
}

static inline pro_item_t
pro_item_uchar(unsigned char const uc)
{
  pro_item_t value;
  value.uc = uc;
  return value;
}

static inline pro_item_t
pro_item_ushort(unsigned short const us)
{
  pro_item_t value;
  value.us = us;
  return value;
}

static inline pro_item_t
pro_item_ulong(unsigned long const ul)
{
  pro_item_t value;
  value.ul = ul;
  return value;
}

static inline pro_item_t
pro_item_ulonglong(unsigned long long const ull)
{
  pro_item_t value;
  value.ull = ull;
  return value;
}

static inline pro_item_t
pro_item_ptr(void *const ptr)
{
  pro_item_t value;
  value.ptr = ptr;
  return value;
}

static inline pro_item_t
pro_item_constptr(void const *const cptr)
{
  pro_item_t value;
  value.cptr = cptr;
  return value;
}

static inline bool
pro_item_are_equal(pro_item_t const u1, pro_item_t const u2)
{
  return u1.u64 == u2.u64;
}

#endif /* PROMOTE_ITEM_H_ */

