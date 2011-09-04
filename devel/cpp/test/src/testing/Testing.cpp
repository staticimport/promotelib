#include "Testing.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

using namespace promote;

double testing::calcNanosPer(double micros, double iters, double count)
{
    return 1000 * micros / count / iters;
}

void testing::printSpeedResult(const std::string& name,
                               const double elapsedMicros)
{
    std::ostringstream oss;
    //oss << std::setprecision(3) << elapsedMicros;
    oss << elapsedMicros;
    const std::string result(oss.str());
    
    std::cout << name;
    for(int ii = 77 - name.length() - result.length(); ii; --ii)
        std::cout << '-';
    std::cout << result << " ns\n";
}

void testing::printUnitResult(const std::string& name,
                              const std::string& error)
{
    const bool isError = error.length() != 0;
    const std::string result = isError ? "fail" : "pass";

    std::cout << name;
    for(int ii = 80 - name.length() - result.length(); ii; --ii)
        std::cout << '-';
    if( isError ) {
        std::cout << "\033[1m" << result << "\033[0m\n\033[7mERROR: " << error << "\033[0m\n";
    } else {
        std::cout << result << std::endl;
    }
}

