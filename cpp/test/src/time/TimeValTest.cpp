
#include "Testing.hpp"
#include "TimeVal.hpp"
#include "TimeValTest.hpp"

using namespace promote;
using namespace promote::testing;

static std::string unitTestXseconds(int64_t (TimeVal::*func)() const,
                                    int64_t const div);
static std::string unitTestSubSeconds(int64_t (TimeVal::*func)() const,
                                      int64_t const div);
static std::string unitTestAdd();
static std::string unitTestSubtract();
static std::string unitTestEquals();
static std::string unitTestNotEquals();
static std::string unitTestLessThan();
static std::string unitTestLessThanOrEq();
static std::string unitTestGreaterThan();
static std::string unitTestGreaterThanOrEq();
static std::string unitTestIncrease();
static std::string unitTestDecrease();

void promote::testing::perfTestTimeVal()
{
}

void promote::testing::unitTestTimeVal()
{
  printUnitResult("TimeVal: microseconds()",
                  unitTestXseconds(&TimeVal::microseconds, 1000));
  printUnitResult("TimeVal: milliseconds()",
                  unitTestXseconds(&TimeVal::milliseconds, 1000000));
  printUnitResult("TimeVal: nanoseconds()",
                  unitTestXseconds(&TimeVal::nanoseconds, 1));
  printUnitResult("TimeVal: seconds()",
                  unitTestXseconds(&TimeVal::seconds, 1000000000));
  printUnitResult("TimeVal: subSecondMicros()",
                  unitTestSubSeconds(&TimeVal::subSecondMicros, 1000));
  printUnitResult("TimeVal: subSecondMillis()",
                  unitTestSubSeconds(&TimeVal::subSecondMillis, 1000000));
  printUnitResult("TimeVal: subSecondNanos()",
                  unitTestSubSeconds(&TimeVal::subSecondNanos, 1));
  printUnitResult("TimeVal: +", unitTestAdd());
  printUnitResult("TimeVal: -", unitTestSubtract());
  printUnitResult("TimeVal: ==", unitTestEquals());
  printUnitResult("TimeVal: !=", unitTestNotEquals());
  printUnitResult("TimeVal: <", unitTestLessThan());
  printUnitResult("TimeVal: <=", unitTestLessThanOrEq());
  printUnitResult("TimeVal: >", unitTestGreaterThan());
  printUnitResult("TimeVal: >=", unitTestGreaterThanOrEq());
  printUnitResult("TimeVal: +=", unitTestIncrease());
  printUnitResult("TimeVal: -=", unitTestDecrease());
}

static std::string unitTestXseconds(int64_t (TimeVal::*func)() const,
                                    int64_t const div)
{
  int64_t const min = -1000 * div;
  int64_t const max = -min;
  int64_t const inc = std::max<int64_t>(1, div / 10);
  for(int64_t nanos = min; nanos != max; nanos += inc) {
    TimeVal const tv(nanos);
    if ((tv.*func)() != nanos / div) {
      return "Incorrect result.";
    }
  }
  return "";
}

static std::string unitTestSubSeconds(int64_t (TimeVal::*func)() const,
                                      int64_t const div)
{
  int64_t const min = -1000 * div;
  int64_t const max = -min;
  int64_t const inc = std::max<int64_t>(1, div / 10);
  for(int64_t nanos = min; nanos != max; nanos += inc) {
    TimeVal const tv(nanos);
    int64_t const seconds = nanos / 1000000000;
    int64_t const remainder = nanos - (seconds * 1000000000);
    if ((tv.*func)() != remainder / div) {
      return "Incorrect result.";
    }
  }
  return "";
}

static std::string unitTestAdd()
{
  TimeVal const tv(0);
  for(int64_t nanos = -10000; nanos != 10000; ++nanos) {
    if ((tv + TimeVal(nanos)).nanoseconds() != nanos) {
      return "Incorrect result.";
    }
  }
  return "";
}

static std::string unitTestSubtract()
{
  TimeVal const tv(0);
  for(int64_t nanos = -10000; nanos != 10000; ++nanos) {
    if ((tv - TimeVal(nanos)).nanoseconds() != -nanos) {
      return "Incorrect result.";
    }
  }
  return "";
}

static std::string unitTestEquals()
{
  for(int64_t ii = -100; ii != 100; ++ii) {
    TimeVal const tv(ii);
    for(int64_t jj = -100; jj != 100; ++jj) {
      if (jj == ii and !(tv == TimeVal(jj))) {
        return "Incorrect result.";
      }
    }
  }
  return "";
}

static std::string unitTestNotEquals()
{
  for(int64_t ii = -100; ii != 100; ++ii) {
    TimeVal const tv(ii);
    for(int64_t jj = -100; jj != 100; ++jj) {
      if (jj != ii and !(tv != TimeVal(jj))) {
        return "Incorrect result.";
      }
    }
  }
  return "";
}

static std::string unitTestLessThan()
{
  for(int64_t ii = -100; ii != 100; ++ii) {
    TimeVal const tv(ii);
    for(int64_t jj = -100; jj != 100; ++jj) {
      if (ii < jj and !(tv < TimeVal(jj))) {
        return "Incorrect result.";
      }
    }
  }
  return "";
}

static std::string unitTestLessThanOrEq()
{
  for(int64_t ii = -100; ii != 100; ++ii) {
    TimeVal const tv(ii);
    for(int64_t jj = -100; jj != 100; ++jj) {
      if (ii <= jj and !(tv <= TimeVal(jj))) {
        return "Incorrect result.";
      }
    }
  }
  return "";
}

static std::string unitTestGreaterThan()
{
  for(int64_t ii = -100; ii != 100; ++ii) {
    TimeVal const tv(ii);
    for(int64_t jj = -100; jj != 100; ++jj) {
      if (ii > jj and !(tv > TimeVal(jj))) {
        return "Incorrect result.";
      }
    }
  }
  return "";
}

static std::string unitTestGreaterThanOrEq()
{
  for(int64_t ii = -100; ii != 100; ++ii) {
    TimeVal const tv(ii);
    for(int64_t jj = -100; jj != 100; ++jj) {
      if (ii >= jj and !(tv >= TimeVal(jj))) {
        return "Incorrect result.";
      }
    }
  }
  return "";
}

static std::string unitTestIncrease()
{
  int64_t nanos = -1000;
  TimeVal tv(nanos);
  for(int64_t ii = -100; ii != 1000; ++ii) {
    nanos += ii;
    tv += ii;
    if (tv.nanoseconds() != nanos) {
      return "Incorrect value.";
    }
  }
  return "";
}

static std::string unitTestDecrease()
{
  int64_t nanos = -1000;
  TimeVal tv(nanos);
  for(int64_t ii = -100; ii != 1000; ++ii) {
    nanos -= ii;
    tv -= ii;
    if (tv.nanoseconds() != nanos) {
      return "Incorrect value.";
    }
  }
  return "";
}


