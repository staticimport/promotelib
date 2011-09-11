#ifndef PROMOTE_SYSTEM_UTILS_H
#define PROMOTE_SYSTEM_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#if defined(__Wp64__) || defined(__alpha__) || defined(__amd64__) || defined(__ppc64__) ||  defined(__ia64__) || defined(__x86_64__)
#ifndef __LP64__
#define __LP64__
#endif
#endif

#ifdef __LP64__
typedef int64_t pro_word_t;
typedef uint64_t pro_uword_t;
static pro_word_t const PRO_WORD_MAX = INT64_MAX;
static pro_word_t const PRO_WORD_MIN = INT64_MIN;
static pro_uword_t const PRO_UWORD_MAX = UINT64_MAX;
static pro_uword_t const PRO_UWORD_MIN = 0;
static bool const PRO_IS_SYSTEM_64BIT = true;
static bool const PRO_WORD_MASK = 63;
static bool const PRO_WORD_BITS = 64;
static bool const PRO_WORD_BYTES = 8;
static bool const PRO_WORD_BYTES_MASK = 7;
#else
typedef int32_t pro_word_t;
typedef uint32_t pro_uword_t;
static pro_word_t const PRO_WORD_MAX = INT32_MAX;
static pro_word_t const PRO_WORD_MIN = INT32_MIN;
static pro_uword_t const PRO_UWORD_MAX = UINT32_MAX;
static pro_uword_t const PRO_UWORD_MIN = 0;
static bool const PRO_IS_SYSTEM_64BIT = false;
static bool const PRO_WORD_MASK = 31;
static bool const PRO_WORD_BITS = 32;
static bool const PRO_WORD_BYTES = 4;
static bool const PRO_WORD_BYTES_MASK = 3;
#endif

#endif /* PROMOTE_SYSTEM_UTILS_H_ */

