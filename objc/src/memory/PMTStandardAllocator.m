#import <stdlib.h>

#import "PMTStandardAllocator.h"

@implementation PMTStandardAllocator

- (id)alloc: (size_t)size zeroOut: (BOOL)zero
{
    return zero ? calloc(1, size) : malloc(size);
}

- (id)release: (id)alloced
{
    free(alloced);
    return self;
}

@end

