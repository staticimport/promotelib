#ifndef PROMOTE_CACHE_LINE_ALLOCATOR_H
#define PROMOTE_CACHE_LINE_ALLOCATOR_H

void* pro_cache_line_alloc(size_t const size, bool const zero_out);
void  pro_cache_line_release(void* alloced);

#endif /* PROMOTE_CACHE_LINE_ALLOCATOR_H */

