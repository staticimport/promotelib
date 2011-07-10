#ifndef PROMOTE_ANY_SIZE_POOL_ALLOCATOR_HPP_
#define PROMOTE_ANY_SIZE_POOL_ALLOCATOR_HPP_

#include <cstdlib>
#include <cstring>

#include "Allocator.hpp"
#include "TreapMap.hpp"

namespace promote
{
    class AnySizePoolAllocator : public Allocator
    {
    private:
        class Block
        {
        public:
            Block(Block *const next, const std::size_t size)
            :   _next(next),
                _allocStart(static_cast<uint8_t*>(malloc(size))),
                _allocEnd(_allocStart + size),
                _free(size)
            { }
            ~Block() { free(_allocStart); }

            uint8_t* alloc(const std::size_t size)
            {
                if( size <= _free ) {
                    uint8_t *const chunk(_allocEnd - _free);
                    _free -= size;
                    return chunk;
                } else {
                    return 0;
                }
            }

            uint8_t* rest(std::size_t& size)
            {
                uint8_t* const rest(_allocEnd - _free);
                size = _free;
                _free = 0;
                return rest;
            }

            std::size_t capacity() const { return _allocEnd - _allocStart; }
            Block* next() const { return _next; }
        private:
            Block *const _next;
            uint8_t *const _allocStart;
            uint8_t *const _allocEnd;
            std::size_t _free;
        };

    public:
        AnySizePoolAllocator(const std::size_t initialSize)
        :   _currentBlock(new Block(0, initialSize))
        { }

        ~AnySizePoolAllocator()
        {
            Block* block(_currentBlock);
            do {
                Block* const next(block->next());
                delete block;
                block = next;
            } while( block );
        }

        void* alloc(std::size_t size, bool zeroOut)
        {
            uint8_t* space(_currentBlock->alloc(size));
            if( !space ) {
                //TODO: get from _freeMap
                if( !space ) {
                    std::size_t restSize(0);
                    //uint8_t* const rest(_currentBlock->rest(restSize));
                    if( restSize ) {
                        // TODO: add to freeMap
                    }
                    _currentBlock = new Block(_currentBlock,
                                              std::max<std::size_t>(size, _currentBlock->capacity() << 1));
                    space = _currentBlock->alloc(size);
                }
            }
            if( zeroOut ) {
                memset(space, 0, size);
            }
            return space;
        }

        void release(void* entry)
        {
            // TODO
        }

    private:
        TreapMap<std::size_t, uint8_t*> _freeMap;
        Block* _currentBlock;
    };
}

#endif /* PROMOTE_ANY_SIZE_POOL_ALLOCATOR_HPP_ */

