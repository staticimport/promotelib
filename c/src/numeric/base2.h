#ifndef PROMOTE_BASE2_H_
#define PROMOTE_BASE2_H_

#include <stdbool.h>
#include <stdint.h>

int pmt_log2_ceil32(uint32_t value);
int pmt_log2_ceil64(uint64_t value);
int pmt_log2_floor32(uint32_t value);
int pmt_log2_floor64(uint64_t value);
uint32_t pmt_pow2_above32(uint32_t value, bool inclusive);
uint64_t pmt_pow2_above64(uint64_t value, bool inclusive);
uint32_t pmt_pow2_below32(uint32_t value, bool inclusive);
uint64_t pmt_pow2_below64(uint64_t value, bool inclusive);

#endif /* PROMOTE_BASE2_H_ */

