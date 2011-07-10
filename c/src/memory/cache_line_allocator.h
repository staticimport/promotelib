#ifndef PROMOTE_CACHE_LINE_ALLOCATOR_H_
#define PROMOTE_CACHE_LINE_ALLOCATOR_H_

void* pmt_cache_line_alloc(size_t const size, bool const zero_out);
void  pmt_cache_line_release(void* alloced);

#endif /* PROMOTE_CACHE_LINE_ALLOCATOR_H_ */

