
#import <Foundation/NSObject.h>

#import "pmt_stopwatch.h"

@interface PMTStopWatch : NSObject
{
    pmt_stopwatch_t stopwatch;
}

- (id)elapsed: (struct timeval*)result;
- (id)reset;
- (id)start;
- (id)stop;

@end

