
#import "PMTStopWatch.h"

@implementation PMTStopWatch

- (id)init
{
    self = [super init];
    if( self ) {
        if( pmt_stopwatch_init(&stopwatch) != 0 ) {
            // TODO?
            [super release];
            return 0;
        }
    }
    return self;
}

- (id)elapsed: (struct timeval*)result
{
    pmt_stopwatch_elapsed(&stopwatch, result);
    return self;
}

- (id)reset
{
    pmt_stopwatch_reset(&stopwatch);
    return self;
}

- (id)start
{
    pmt_stopwatch_start(&stopwatch);
    return self;
}

- (id)stop
{
    pmt_stopwatch_stop(&stopwatch);
    return self;
}

@end

