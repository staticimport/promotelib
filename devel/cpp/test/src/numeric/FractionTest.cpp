
#include <iostream>

#include "Fraction.hpp"
#include "FractionTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

static std::string basicTest();

void testing::perfTestFraction()
{
    std::cout << "\n\033[1m[ Fraction ]\033[0m\n";
}

void testing::unitTestFraction()
{
    std::cout << "\n\033[1m[ Fraction ]\033[0m\n";
    testing::printUnitResult("basic test", basicTest());
}

static std::string basicTest()
{
    if( Fraction<int>(1,2).numerator() != 1 ) {
        return "Failed on numerator of non-reduceable fraction.";
    } else if( Fraction<int>(1,2).denominator() != 2 ) {
        return "Failed on denominator of non-reduceable fraction.";
    } else if( Fraction<int>(100,10).numerator() != 10 ) {
        return "Failed on nuemrator of reduceable fraction.";
    } else if( Fraction<int>(100,10).denominator() != 1 ) {
        return "Failed on denominator of reduceable fraction.";
    }

    return "";
}
