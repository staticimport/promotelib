#ifndef PROMOTE_COMPATABILITY_H
#define PROMOTE_COMPATABILITY_H

// Support for likely/unlikely only in Linux
#ifdef Linux
#include <linux/compiler.h>
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#endif /* PROMOTE_COMPATABILITY_H_ */

