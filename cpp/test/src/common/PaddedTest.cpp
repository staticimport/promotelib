
#include "Padded.hpp"
#include "PaddedTest.hpp"
#include "testing.hpp"

using namespace promote;
using namespace promote::testing;

static std::string unitTestPaddedCount();

template <std::size_t L>
struct Foo {
  uint8_t array[L];
};

void promote::testing::unitTestPadded()
{
  printUnitResult("Padded: sizeof", unitTestPaddedCount());
}

static std::string unitTestPaddedCount()
{
  if (sizeof(Padded<Foo<1> >) != 1) {
    return "Incorrect size 1 Padded";
  } else if (sizeof(Padded<Foo<2> >) != 2) {
    return "Incorrect size 2 Padded";
  } else if (sizeof(Padded<Foo<3> >) != 4) {
    return "Incorrect size 3 Padded";
  } else if (sizeof(Padded<Foo<4> >) != 4) {
    return "Incorrect size 4 Padded";
  } else if (sizeof(Padded<Foo<5> >) != 8) {
    return "Incorrect size 5 Padded";
  } else if (sizeof(Padded<Foo<6> >) != 8) {
    return "Incorrect size 6 Padded";
  } else if (sizeof(Padded<Foo<7> >) != 8) {
    return "Incorrect size 7 Padded";
  } else if (sizeof(Padded<Foo<8> >) != 8) {
    return "Incorrect size 8 Padded";
  } else if (sizeof(Padded<Foo<9> >) != 16) {
    return "Incorrect size 9 Padded";
  } else if (sizeof(Padded<Foo<16> >) != 16) {
    return "Incorrect size 16 Padded";
  } else if (sizeof(Padded<Foo<17> >) != 24) {
    return "Incorrect size 17 Padded";
  }
  return "";
}

