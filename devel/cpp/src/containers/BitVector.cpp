
#include <cstring>
#include <iostream>

#include "BitVector.hpp"

using namespace promote;

BitVector::BitVector(std::size_t const initCapacity)
:   _capacity(std::max<std::size_t>(initCapacity, 8)), // at least 1 byte
    _bytes(static_cast<uint8_t*>(calloc(_capacity, 1)))
{
}

BitVector::~BitVector()
{
    free(_bytes);
}

