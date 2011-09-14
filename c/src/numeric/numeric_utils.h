#ifndef PROMOTE_NUMERIC_UTILS_H
#define PROMOTE_NUMERIC_UTILS_H

#include <stdbool.h>
#include <stdint.h>

// Logarithm (base 2)
unsigned pro_log2_ceil32(uint32_t value);
unsigned pro_log2_ceil64(uint64_t value);
unsigned pro_log2_floor32(uint32_t value);
unsigned pro_log2_floor64(uint64_t value);

// Exponentiation (base 2)
uint32_t pro_pow2_above32(uint32_t value, bool const inclusive);
uint64_t pro_pow2_above64(uint64_t value, bool const inclusive);
uint32_t pro_pow2_below32(uint32_t value, bool const inclusive);
uint64_t pro_pow2_below64(uint64_t value, bool const inclusive);

#endif /* PROMOTE_NUMERIC_UTILS_H */

