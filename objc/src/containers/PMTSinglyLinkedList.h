#import <Foundation/NSObject.h>

#import "PMTCommonType.h"

typedef struct pmt_sllist_node {
    struct pmt_sllist_node* next;
    pmt_common_t value;
} pmt_sllist_node_t;

@interface PMTSinglyLinkedList : NSObject
{
    struct pmt_sllist_node* _first;
	struct pmt_sllist_node* _last;
	NSUInteger _count;
}

- (NSUInteger)count;

- (pmt_common_t)atIndex:(NSUInteger)index;
- (BOOL)contains:(pmt_common_t)item;
- (BOOL)containsObject:(id)object;
- (pmt_common_t)first;
- (id)firstObject;
- (pmt_common_t)last;
- (id)lastObject;
- (id)objectAtIndex:(NSUInteger)index;


@end

