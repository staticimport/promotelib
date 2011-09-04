#ifndef PROMOTE_STANDARD_ALLOCATOR_HPP_
#define PROMOTE_STANDARD_ALLOCATOR_HPP_

#include <cstdlib>

#include "Allocator.hpp"

namespace promote
{
    class StandardAllocator : public Allocator
    { 
    public:
        StandardAllocator()  { }
        ~StandardAllocator() { }

        static StandardAllocator& instance()
        {
            static StandardAllocator instance;
            return instance;
        }

        void* alloc(std::size_t size, bool zeroOut)
        {
            return !zeroOut ? malloc(size) : calloc(1, size);
            /*void* space = ::operator new(size);
            if( space and zeroOut )
	            memset(space, 0, size);
            return space;*/
        }

        void release(void* alloced)
        {
            free(alloced);
            //::operator delete(alloced);
        }
    };

    class NewStandardAllocator
    {
    public:
        NewStandardAllocator(std::size_t const ignored=0) { }

        void* allocate(std::size_t const size)  { return malloc(size); }
        void deallocate(void* const allocated)  { free(allocated); }
    };
}

#endif /* PROMOTE_STANDARD_ALLOCATOR_HPP_ */
