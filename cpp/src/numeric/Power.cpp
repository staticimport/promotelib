#include "Power.hpp"

uint64_t
promote::leastPower2Above64(uint64_t value,
                            bool inclusive)
{
    uint64_t result(inclusive ? value - 1 : value);
    result |= result >> 1;
    result |= result >> 2;
    result |= result >> 4;
    result |= result >> 8;
    result |= result >> 16;
    result |= result >> 32;
    return result + 1;
}

