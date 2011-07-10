#ifndef PROMOTE_POOL_QUEUE_ALLOCATOR_HPP_
#define PROMOTE_POOL_QUEUE_ALLOCATOR_HPP_

#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <stdint.h>

#include "Allocator.hpp"
#include "Array.hpp"
#include "Queue.hpp"

namespace promote
{
    template <bool fixedsize=false, bool rwsafe=false>
    class PoolQueueAllocator : public Allocator
    {
    public:
        PoolQueueAllocator(const std::size_t entrySize,
                           const std::size_t initCount)
        :   _entrySize(entrySize),
            _queue(initCount),
            _alloced(static_cast<void*>(0)),
            _allocedCount(0),
            _lastExpandCount(initCount >> 1)
        { 
            expand();
            /*while( _newEntries.nextNewEntry != _newEntries.endOfNewEntries ) {
                _queue.write(_newEntries.nextNewEntry);
                _newEntries.nextNewEntry += _entrySize;
            }*/
        }

        ~PoolQueueAllocator()
        {
            for(std::size_t ii = 0; ii != _allocedCount; ++ii) {
                free(_alloced[ii]);
            }
        }

        void* alloc(std::size_t size, bool zeroOut)
        {
            void* space(0);
            if( size <= _entrySize ) {
                if( !_queue.read(space) ) {
                    if( _newEntries.nextNewEntry == _newEntries.endOfNewEntries ) {
                        expand();
                        if( _newEntries.nextNewEntry != _newEntries.endOfNewEntries ) {
                            space = _newEntries.nextNewEntry;
                            _newEntries.nextNewEntry += _entrySize;
                        }
                    } else {
                        space = _newEntries.nextNewEntry;
                        _newEntries.nextNewEntry += _entrySize;
                    }
                }
                if( zeroOut and space ) {
                    memset(space, 0, size);
                }
            }
            return space;
        }

        void release(void* entry)
        {
            _queue.write(entry);
        }
    private:
        void expand()
        {
            const std::size_t count(std::max<std::size_t>(1,_lastExpandCount << 1));
            std::size_t const byteCount = _entrySize * count;
            uint8_t* block = static_cast<uint8_t*>(malloc(byteCount));
            if( block ) {
                _alloced[_allocedCount++] = block; 
                _lastExpandCount = count;
                _newEntries.nextNewEntry = block;
                _newEntries.endOfNewEntries = block + byteCount;
            } else {
                _newEntries.nextNewEntry = _newEntries.endOfNewEntries = 0;
            }
        }

        const std::size_t _entrySize;
        Queue<void*,fixedsize,rwsafe> _queue;
        StaticArray<void*,64> _alloced;
        struct {
            uint8_t* nextNewEntry;
            uint8_t* endOfNewEntries;
        } _newEntries;
        std::size_t _allocedCount;
        std::size_t _lastExpandCount;
    };
    
    template <bool rwsafe>
    class PoolQueueAllocator<true,rwsafe> : public Allocator
    {
    public:
        PoolQueueAllocator(const std::size_t entrySize,
                           const std::size_t count)
        :   _entrySize(entrySize),
            _queue(count),
            _alloced(static_cast<uint8_t*>(malloc(entrySize * count)))
        { 
            uint8_t* entry = _alloced;
            for(std::size_t ii = 0; ii != count; ++ii) {
                _queue.write(entry);
                entry += entrySize;
            }
        }

        ~PoolQueueAllocator()
        {
            free(_alloced);
        }

        void* alloc(std::size_t size, bool zeroOut)
        {
            void* space(0);
            if( size <= _entrySize ) {
                if( _queue.read(space) and zeroOut ) {
                    memset(space, 0, size);
                }
            }
            return space;
        }

        void release(void* entry)
        {
            _queue.write(entry);
        }
    private:
        const std::size_t _entrySize;
        Queue<void*,true,rwsafe> _queue;
        uint8_t* const _alloced;
    };
}

#endif /* PROMOTE_POOL_QUEUE_ALLOCATOR_HPP_ */
