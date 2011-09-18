
#include <iostream>
#include <string>
#include <unordered_set>

#include "StopWatchTest.hpp"
#include "TimeValTest.hpp"

#define CONDITIONAL_RUN_TEST_SUITE(name) \
  if (args.empty() or args.find(#name) != args.end()) { \
    std::cout << "\n[[ src/" << #name << " ]]\n"; \
    name ## Test(); \
  }

using namespace promote::testing;

static void timeTest();

int main(int argc, char const* const* argv)
{
  // Args
  std::unordered_set<std::string> args;
  for(int ii = 1; ii < argc; ++ii) {
    args.insert(argv[ii]);
  }

  // Tests
  CONDITIONAL_RUN_TEST_SUITE(time)

  return 0;
}

static void timeTest()
{
  unitTestStopWatch();
  unitTestTimeVal();
}

