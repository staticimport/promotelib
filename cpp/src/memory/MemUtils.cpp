#include "MemUtils.hpp"

#include <cstdio>
#include <cstring>
#include <stdint.h>

void promote::memzero(void* mem, std::size_t size)
{
    /*if( size < 100 ) {
        const std::size_t count64 = size >> 3;
        const std::size_t count8 = size & 7;
        std::size_t ii(0);
        uint64_t* parts64 = static_cast<uint64_t*>(mem);
        uint8_t* const parts8(static_cast<uint8_t*>(mem) + (size - count8));
    
        for(ii = 0; ii != count64; ++ii) {
            parts64[ii] = 0;
        }
        for(ii = 0; ii < count8; ++ii)
            parts8[ii] = 0;
    } else*/
        memset(mem, 0, size);
}

