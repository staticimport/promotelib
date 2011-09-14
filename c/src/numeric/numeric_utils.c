
#include <assert.h>

#include "numeric_utils.h"

unsigned pro_log2_ceil32(uint32_t value)
{
  unsigned const floor = pro_log2_floor32(value);
  return ((1 << floor) != value ? floor + 1 : floor);
}

unsigned pro_log2_ceil64(uint64_t value)
{
  unsigned const floor = pro_log2_floor64(value);
  return ((1 << floor) != value ? floor + 1 : floor);
}

unsigned pro_log2_floor32(uint32_t value)
{
  assert(value);
  unsigned result = 0;
  if (value & 0xffff0000) { value >>= 16; result += 16; }
  if (value & 0x0000ff00) { value >>= 8;  result += 8;  }
  if (value & 0x000000f0) { value >>= 4;  result += 4;  }
  if (value & 0x0000000c) { value >>= 2;  result += 2;  }
  if (value & 0x00000002) { ++result;  }
  return result;
}

unsigned pro_log2_floor64(uint64_t value)
{
  assert(value);
  unsigned result = 0;
  if (value & 0xffffffff00000000) { value >>= 32; result += 32; }
  if (value & 0x00000000ffff0000) { value >>= 16; result += 16; }
  if (value & 0x000000000000ff00) { value >>= 8;  result += 8;  }
  if (value & 0x00000000000000f0) { value >>= 4;  result += 4;  }
  if (value & 0x000000000000000c) { value >>= 2;  result += 2;  }
  if (value & 0x0000000000000002) { ++result;  }
  return result;
}

uint32_t pro_pow2_above32(uint32_t value, bool const inclusive)
{
  if (inclusive) --value;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  return value + 1;
}

uint64_t pro_pow2_above64(uint64_t value, bool const inclusive)
{
  if (inclusive) --value;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  value |= value >> 32;
  return value + 1;
}

uint32_t pro_pow2_below32(uint32_t value, bool const inclusive)
{
  assert(value);
  if (value & 0xfffffffc) {
    value >>= 1;
    if (inclusive) ++value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
  } else {
    switch (value) {
      case 3: return 2;
      case 2: return inclusive ? 2 : 1;
      case 1: assert(inclusive); return 1;
      default: assert(false); return 0;
    }
  }
}

uint64_t pro_pow2_below64(uint64_t value, bool const inclusive)
{
  assert(value);
  if (value & 0xfffffffffffffffc) {
    value >>= 1;
    if (inclusive) ++value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return value + 1;
  } else {
    switch (value) {
      case 3: return 2;
      case 2: return inclusive ? 2 : 1;
      case 1: assert(inclusive); return 1;
      default: assert(false); return 0;
    }
  }
}

