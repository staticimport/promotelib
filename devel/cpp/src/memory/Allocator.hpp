#ifndef PROMOTE_ALLOCATOR_HPP_
#define PROMOTE_ALLOCATOR_HPP_

#include <cstddef>

namespace promote
{
    class Allocator
    {
    public:
        virtual ~Allocator() { }
        
        virtual void* alloc(std::size_t size, bool zero_out) = 0;
        virtual void  release(void *alloced) = 0;
    };
}

#endif /* PROMOTE_ALLOCATOR_HPP_ */
