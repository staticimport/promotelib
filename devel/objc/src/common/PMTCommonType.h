
#import <Foundation/NSObject.h>

// TODO: check sizeof(id) == 8
enum pmt_common_type {
    PMT_COMMON_FLOAT, PMT_COMMON_INT, PMT_COMMON_OBJ
};

typedef struct pmt_common {
    union {
        float f;
		id obj;
        int i;
    } value;
    enum pmt_common_type type;
} pmt_common_t;

@interface PMTCommonType : NSObject
{
}

+ (pmt_common_t)fromFloat: (float)x;
+ (pmt_common_t)fromInt: (int)x;
+ (pmt_common_t)fromObject: (id)x;

+ (float)toFloat: (pmt_common_t)x;
+ (int)toInt: (pmt_common_t)x;
+ (id)toObject: (pmt_common_t)x;

@end
