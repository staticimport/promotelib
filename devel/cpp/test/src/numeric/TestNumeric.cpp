
#include "FractionTest.hpp"
#include "IntegralMultiplyTest.hpp"
#include "PowerTest.hpp"

int main(void)
{
    promote::testing::unitTestFraction();
    promote::testing::testIntegralMultiply();
    promote::testing::testPower();
    return 0;
}

