#import <Foundation/NSObject.h>

@protocol PMTAllocator

- (id)alloc: (size_t)size zeroOut: (BOOL)zero;
- (id)release: (id)alloced;

@end

