
#include <algorithm>
#include <stdint.h>
#include <cstdio>

#include "MemUtils.hpp"
#include "PoolAllocator.hpp"

namespace promote
{
    class PoolAllocBlock
    {
    public:
        ~PoolAllocBlock() { ::operator delete(_alloced); }

    private:
        PoolAllocBlock(std::size_t entrySize, std::size_t total, PoolAllocBlock* nextBlock) 
        {
            _nextBlock = nextBlock;
            _entrySize = entrySize;
            _alloced = static_cast<uint8_t*>(::operator new(entrySize * total));
            _nextEntry = _alloced;
            _end = _alloced + (entrySize * total);
        }
        
        //    _nextBlock(nextBlock),
        //    _entrySize(entrySize),
        //    _alloced(static_cast<uint8_t*>(::operator new(entrySize * total))),
        //    _nextEntry(_alloced),
        //    _end(_alloced + (entrySize * total))
        //{}

    bool hasRemaining() { return _nextEntry != _end; }
    void* next() 
    {
        uint8_t* const entry = _nextEntry;
        _nextEntry += _entrySize;
        return static_cast<void*>(entry);
    }

    PoolAllocBlock* _nextBlock;
    std::size_t _entrySize;
    uint8_t* _alloced;
    uint8_t* _nextEntry;
    uint8_t* _end;
  
    friend class PoolAllocator;
  };
}

promote::PoolAllocator::~PoolAllocator()
{
  PoolAllocBlock* block = _allocedList;
  PoolAllocBlock* temp;
  while(block) {
    temp = block->_nextBlock;
    delete block;
    block = temp;
  }
  _allocedList = 0;
}

void* promote::PoolAllocator::alloc(std::size_t size, bool zeroOut)
{
    if( size <= _entrySize ) {
        void* entry(0);
        if( _freeList ) {
            PoolEntry* poolEntry = _freeList;
            _freeList = poolEntry->nextEntry;
            entry = poolEntry;
        } else if( _allocedList->hasRemaining() ) {
            entry = _allocedList->next();
        } else if( _entryCount != _maxCount ) {
            expand(std::min<std::size_t>(_maxCount - _entryCount, _entryCount));
            entry = _allocedList->next();
        }

        if( zeroOut and entry ) {
            memset(entry, 0, size);
        }
        return entry;
    }
    return 0;
}

void promote::PoolAllocator::expand(std::size_t growCount)
{
    _allocedList = new PoolAllocBlock(_entrySize, growCount, _allocedList);
    _entryCount += growCount;
}

