#ifndef PROMOTE_FIXED_POOL_ALLOCATOR
#define PROMOTE_FIXED_POOL_ALLOCATOR

#include "Stack.hpp"

namespace promote
{
    template <typename FreeList=Stack<void*>, typename AllocedList=Stack<void*> >
    class FixedPoolAllocator
    {
    public:
        FixedPoolAllocator(std::size_t const entrySize);
        ~FixedPoolAllocator();

        void* allocate(std::size_t const size) {
            if( size <= _entrySize ) {
                if( !_freeList.empty() or (expand() and !_freeList.empty()) ) {
                    void* const ptr(*_freeList.begin());
                    _freeList.pop();
                    return ptr;
                }
            }
            return NULL;
        }

        void deallocate(void* const allocated) {
            _freeList.push(allocated); 
        }
    private:
        bool expand();

        static std::size_t roundEntrySize(std::size_t const size) {
            if( size > 4 ) {
                std::size_t const sizeMod8(size % 8);
                return 8 * (size / 8) + (sizeMod8 ? 8 - sizeMod8 : 0);
            } else if( size > 2 ) {
                std::size_t const sizeMod4(size % 4);
                return 4 * (size / 4) + (sizeMod4 ? 4 - sizeMod4 : 0);
            } else {
                return size;
            }
        }


        std::size_t const _entrySize;
        AllocedList _allocedList;
        FreeList _freeList;
    };
}

/**
 * Implementation
 **/
template <typename FreeList, typename AllocedList>
promote::FixedPoolAllocator<FreeList, AllocedList>::FixedPoolAllocator(std::size_t const entrySize)
:   _entrySize(roundEntrySize(entrySize))
{
}

template <typename FreeList, typename AllocedList>
promote::FixedPoolAllocator<FreeList, AllocedList>::~FixedPoolAllocator()
{
    for(AllocedList::Iterator iter = _allocedList.begin();
        iter != _allocedList.end();
        ++iter)
    {
        free(*iter);
    }
}

template <typename FreeList, typename AllocedList>
bool promote::FixedPoolAllocator<FreeList, AllocedList>::expand()
{
    std::size_t const count(16 << _allocedList.size());
    std::size_t const allocSize(count * _entrySize);
    char* const alloced(static_cast<char*>(malloc(allocSize)));
    char* const end(alloced + allocSize);
    for(std::size_t entry = alloced; entry != end; entry += _entrySize) {
        _freeList.push(entry);
    }
    return true;
}

#endif /* PROMOTE_FIXED_POOL_ALLOCATOR */

