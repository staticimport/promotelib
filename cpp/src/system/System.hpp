#ifndef PROMOTE_SYSTEM_HPP_
#define PROMOTE_SYSTEM_HPP_

#define __STDC_LIMIT_MACROS

#include <cstddef>
#include <limits>
#include <stdint.h>

#if defined(__Wp64__) || defined(__alpha__) || defined(__amd64__) || defined(__ppc64__) ||  defined(__ia64__) || defined(__x86_64__)
#ifndef __LP64__
#define __LP64__
#endif
#endif

#ifndef UINT32_MAX
#define UINT32_MAX (0xffffffffUL)
#endif

#ifndef UINT32_MIN
#define UINT32_MIN (0)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX (0xffffffffffffffffULL)
#endif

#ifndef UINT64_MIN
#define UINT64_MIN (0)
#endif

namespace promote
{
#ifdef __LP64__
    typedef int64_t word_t;
    typedef uint64_t uword_t;
    static bool const IS_SYSTEM_64BIT = true;
    static std::size_t const WORD_BITS = 64;
    static std::size_t const WORD_BITS_LOG2 = 6;
    static std::size_t const WORD_BYTES = 8;
    static std::size_t const WORD_BYTES_MASK = 7;
#else
    typedef int32_t word_t;
    typedef uint32_t uword_t;
    static bool const IS_SYSTEM_64BIT = false;
    static std::size_t const WORD_BITS = 32;
    static std::size_t const WORD_BITS_LOG2 = 5;
    static std::size_t const WORD_BYTES = 4;
    static std::size_t const WORD_BYTES_MASK = 3;
#endif
}

#endif /* PROMOTE_SYSTEM_HPP_ */

