#ifndef PROMOTE_SYSTEM_UTILS_H_
#define PROMOTE_SYSTEM_UTILS_H_

#include <stdbool.h>
#include <stdint.h>

#if defined(__Wp64__) || defined(__alpha__) || defined(__amd64__) || defined(__ppc64__) ||  defined(__ia64__) || defined(__x86_64__)
#ifndef __LP64__
#define __LP64__
#endif
#endif

#ifdef __LP64__
typedef int64_t pmt_word_t;
typedef uint64_t pmt_uword_t;
static pmt_word_t const PMT_WORD_MAX = INT64_MAX;
static pmt_word_t const PMT_WORD_MIN = INT64_MIN;
static pmt_uword_t const PMT_UWORD_MAX = UINT64_MAX;
static pmt_uword_t const PMT_UWORD_MIN = 0;
static bool const PMT_IS_SYSTEM_64BIT = true;
static bool const PMT_WORD_MASK = 63;
static bool const PMT_WORD_BITS = 64;
static bool const PMT_WORD_BYTES = 8;
static bool const PMT_WORD_BYTES_MASK = 7;
#else
typedef int32_t pmt_word_t;
typedef uint32_t pmt_uword_t;
static pmt_word_t const PMT_WORD_MAX = INT32_MAX;
static pmt_word_t const PMT_WORD_MIN = INT32_MIN;
static pmt_uword_t const PMT_UWORD_MAX = UINT32_MAX;
static pmt_uword_t const PMT_UWORD_MIN = 0;
static bool const PMT_IS_SYSTEM_64BIT = false;
static bool const PMT_WORD_MASK = 31;
static bool const PMT_WORD_BITS = 32;
static bool const PMT_WORD_BYTES = 4;
static bool const PMT_WORD_BYTES_MASK = 3;
#endif

#endif /* PROMOTE_SYSTEM_UTILS_H_ */

