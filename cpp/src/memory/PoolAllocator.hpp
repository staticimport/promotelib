#ifndef PROMOTE_POOL_ALLOCATOR_HPP_
#define PROMOTE_POOL_ALLOCATOR_HPP_

#include <cstddef>
#include <cstring>
#include <limits>

#include "Allocator.hpp"

namespace promote
{
    class PoolAllocBlock;
 
    struct PoolEntry {
        PoolEntry* nextEntry;
    };

    class PoolAllocator : public Allocator
    {
    public:
        PoolAllocator(size_t entrySize, size_t initCount=8, size_t maxCount=std::numeric_limits<std::size_t>::max()) :
            _entrySize(entrySize >= sizeof(PoolEntry) ? entrySize : sizeof(PoolEntry)), 
            _entryCount(0), _maxCount(maxCount), _allocedList(0), _freeList(0)
        {
            expand(initCount);
        }
        ~PoolAllocator();

        void* alloc(std::size_t size, bool zeroOut);
    
        void release(void* alloced)
        {
            PoolEntry* entry = static_cast<PoolEntry*>(alloced);
            entry->nextEntry = _freeList;
            _freeList = entry;
        }

        std::size_t entrySize()  { return _entrySize; }
        std::size_t entryCount() { return _entryCount; }
        std::size_t maxCount()   { return _maxCount; }

  private:
    void expand(std::size_t growCount);

    std::size_t      _entrySize;
    std::size_t      _entryCount;
    std::size_t      _maxCount;
    PoolAllocBlock*  _allocedList;
    PoolEntry*       _freeList;
  };
}

#endif /* PROMOTE_POOL_ALLOCATOR_HPP_ */
