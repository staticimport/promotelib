#ifndef PROMOTE_CACHE_LINE_ALLOCATOR_HPP_
#define PROMOTE_CACHE_LINE_ALLOCATOR_HPP_

#include <cstdlib>
#include <cstring>
#include <stdint.h>

#include "Allocator.hpp"

namespace promote
{
    class CacheLineAllocator : public Allocator
    {
    public:
        static std::size_t const CACHE_LINE_SIZE_LOG2 = 6;
        static std::size_t const CACHE_LINE_SIZE = 
                1 << CACHE_LINE_SIZE_LOG2;
        static std::size_t const CACHE_LINE_MASK = 
                CACHE_LINE_SIZE - 1;

        static CacheLineAllocator& instance()
        {
            static CacheLineAllocator instance;
            return instance;
        }

        CacheLineAllocator()  { }
        ~CacheLineAllocator() { }

        void* alloc(std::size_t size, bool zeroOut=false)
        {
            // Smallest multiple of 64 >= size, plus 1
            std::size_t const sizeDivLine(size >> CACHE_LINE_SIZE_LOG2);
            std::size_t const sizeModLine(size & CACHE_LINE_MASK);
            std::size_t const extraLines(sizeModLine ? 2 : 1);
            std::size_t const numLines(sizeDivLine + extraLines);

            // Alloc
            uint8_t* const alloced(static_cast<uint8_t*>(malloc(numLines << CACHE_LINE_SIZE_LOG2)));
            std::size_t const toNextLine(CACHE_LINE_SIZE - (reinterpret_cast<std::size_t>(alloced) & CACHE_LINE_MASK));
            uint8_t const offset(static_cast<uint8_t>(toNextLine ? toNextLine : CACHE_LINE_SIZE));
            uint8_t* const toReturn(alloced + offset);
            //printf("Alloced=%x, Offset=%u, ToReturn=%x\n", alloced, (unsigned)offset, toReturn);

            // Insert offset
            *(toReturn-1) = offset;

            // Zero out
            if( zeroOut ) {
                memset(toReturn, 0, size);
            }

            // Return
            return toReturn;
        }

        void release(void* alloced)
        {
            uint8_t* const allocedBytes(static_cast<uint8_t*>(alloced));
            uint8_t const offset(*(allocedBytes-1));
            //printf("ToFree=%x, Offset=%u, Returned=%x\n", allocedBytes-offset, (unsigned)offset, alloced);
            free(allocedBytes - offset);
        }
    };
}

#endif /* PROMOTE_CACHE_LINE_ALLOCATOR_HPP_ */

