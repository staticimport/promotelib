#ifndef PROMOTE_TESTING_HPP_
#define PROMOTE_TESTING_HPP_

#include <string>
#include <sys/time.h>

namespace promote
{
    namespace testing {
        const unsigned STANDARD_SPEED_COUNT = 1000;
        const unsigned STANDARD_SPEED_ITERS = 10000;

        struct TestTimes {
            struct timeval min;
            struct timeval average;
            struct timeval max;
        };

        double calcNanosPer(double micros,
                            double iters = STANDARD_SPEED_ITERS,
                            double count = STANDARD_SPEED_COUNT);
        void printSpeedResult(const std::string& name,
                              const double elapsedMicros);
        void printUnitResult(const std::string& name,
                             const std::string& error);
    };
}

#endif /* PROMOTE_TESTING_HPP_ */
