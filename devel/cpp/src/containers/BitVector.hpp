#ifndef PROMOTE_BIT_VECTOR_HPP_
#define PROMOTE_BIT_VECTOR_HPP_

#include <cstdlib>
#include <cstring>
#include <limits>

#include "Array.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "System.hpp"

namespace promote
{
    class BitVector
    {
    public:
        class SetBitsIterator : public promote::Iterator<std::size_t>
        {
        public:
            struct Exception : public promote::Exception {
                Exception(String const& reason) : promote::Exception(reason) { }
            };

            SetBitsIterator(BitVector const& bitVector) 
            :   _bitVector(bitVector), _byteIndex(0), _withinByteIndex(0) 
            { }
            ~SetBitsIterator() { }

            bool hasNext() {
                if( _withinByteIndex ) {
                    std::size_t const baseIndex(_byteIndex << 3);
                    for(; _withinByteIndex < 8; ++_withinByteIndex) {
                        if( _bitVector[baseIndex + _withinByteIndex] ) {
                            return true;
                        }
                    }
                    _withinByteIndex = 0;
                    ++_byteIndex;
                }
                std::size_t const byteCapacity(_bitVector._capacity >> 3);
                for(; _byteIndex < byteCapacity; ++_byteIndex) {
                    if( _bitVector._bytes[_byteIndex] ) {
                        return true;
                    }
                }
                return false;
            }
            std::size_t next() {
                if( hasNext() ) {
                    std::size_t const baseIndex(_byteIndex << 3);
                    while( _withinByteIndex < 8 ) {
                        std::size_t const index(baseIndex + _withinByteIndex++);
                        if( _bitVector[index] ) {
                            return index;
                        }
                    }
                    throw Exception("No set bits found in byte as expected after hasNext()!");
                } else {
                    throw Exception("Reached end of BitVector.");
                }
            }
        private:
            BitVector const& _bitVector;
            std::size_t _byteIndex;
            std::size_t _withinByteIndex;
        };

        BitVector(std::size_t const initCapacity=64);
        ~BitVector();

        // Const
        bool operator[](std::size_t const index) const
        {
            if( index < _capacity ) {
                return ((_bytes[byteIndex(index)] >> withinByteIndex(index)) & 1) == 1;
            } else {
                return false;
            }
        }

        // Non-Const
        void clear(std::size_t const index) {
            if( index < _capacity ) {
                uint8_t byteMask(~(1 << static_cast<uint8_t>(withinByteIndex(index))));
                _bytes[byteIndex(index)] &= byteMask;
            }
        }
        void set(std::size_t const index) {
            if( index >= _capacity ) {
                std::size_t newCapacity(_capacity << 1);
                while( index >= newCapacity ) {
                    newCapacity <<= 1;
                }
                _bytes = static_cast<uint8_t*>(realloc(_bytes, newCapacity >> 3));
                memset(_bytes + (_capacity >> 3), 0, (newCapacity - _capacity) >> 3);
                _capacity = newCapacity;
            }
            uint8_t byteMask(1 << static_cast<uint8_t>(withinByteIndex(index)));
            _bytes[byteIndex(index)] |= byteMask;
        }
    private:
        static std::size_t byteIndex(std::size_t const rawIndex) { return (rawIndex >> 3); }
        static std::size_t withinByteIndex(std::size_t const rawIndex) { return (rawIndex & 7); }

        std::size_t _capacity;
        uint8_t* _bytes;
    };
    /*class BitVector
    {
    public:
        BitVector(std::size_t const initCapacity=16);
        ~BitVector();

        // Const
        bool operator[](std::size_t const index) const
        {
            std::size_t const wordIndex(getWordIndex(index));
            return wordIndex < _array->length() ?
                ((*_array)[wordIndex] & (static_cast<uword_t>(1) << getInWordIndex(index))) != 0 : false;
        }

        // Non-Const
        void clear(std::size_t const index)
        {
            std::size_t const wordIndex(getWordIndex(index));
            if( wordIndex < _array->length() ) {
                (*_array)[wordIndex] &= (std::numeric_limits<uword_t>::max() - 
                                        (static_cast<uword_t>(1) << getInWordIndex(index)));
            }
        }
        bool expand(std::size_t const min_capacity);
        void set(std::size_t const index)
        {
            std::size_t const wordIndex(getWordIndex(index));
            if( wordIndex < _array->length() or expand(index+1) ) {
                (*_array)[wordIndex] |= static_cast<uword_t>(1) << getInWordIndex(index);
            }
        }
    private:
        static std::size_t getInWordIndex(std::size_t const index) {
            return index & std::numeric_limits<uword_t>::max();
        }
        static std::size_t getWordIndex(std::size_t const index) {
            return index >> WORD_BITS_LOG2;
        }

        Array<uword_t>* _array;
        std::size_t _numBitsSet;
    };*/
}

#endif /* PROMOTE_BIT_VECTOR_HPP_ */

