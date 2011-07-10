
#import "PMTCommonType.h"

@implementation PMTCommonType

+ (pmt_common_t)fromFloat: (float)x
{
    pmt_common_t common;
    common.value.f = x;
    common.type = PMT_COMMON_FLOAT;
    return common;
}

+ (pmt_common_t)fromInt: (int)x
{
    pmt_common_t common;
    common.value.i = x;
    common.type = PMT_COMMON_INT;
    return common;
}

+ (pmt_common_t)fromObject: (id)x
{
	pmt_common_t common;
	common.value.obj = x;
	common.type = PMT_COMMON_OBJ;
	return common;
}
	
+ (float)toFloat: (pmt_common_t)x
{
    return x.value.f;
}

+ (int)toInt: (pmt_common_t)x
{
    return x.value.i;
}

+ (id)toObject: (pmt_common_t)x
{
	return x.value.obj;
}

@end

