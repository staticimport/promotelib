#import <Foundation/NSObject.h>

#import "PMTAllocator.h"

@interface PMTStandardAllocator : NSObject<PMTAllocator>
{
}

- (id)alloc: (size_t)size zeroOut: (BOOL)zero;
- (id)release: (id)alloced;

@end

