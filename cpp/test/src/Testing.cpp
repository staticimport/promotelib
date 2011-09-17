
#include "Testing.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>


void promote::testing::printPerfResult(std::string const& name, double const nanos)
{
  std::ostringstream oss;
  oss << std::setprecision(3) << nanos;
  std::string const nanosStr(oss.str());

  std::cout << name;
  for(std::size_t ii = 77 - name.length() - nanosStr.length(); ii; --ii) {
    std::cout << '-';
  }
  std::cout << nanosStr << " ns\n";
}

void promote::testing::printPerfItersResult(std::string const& name,
                                            std::size_t const iters,
                                            double const nanos)
{
  std::ostringstream oss1;
  oss1 << name << " x " << iters;
  std::string const nameAndItersStr(oss1.str());

  std::ostringstream oss2;
  oss2 << "total: " << (uint64_t)nanos << ", avg: " << std::setprecision(3) << nanos;
  std::string const nanosStr(oss2.str());

  std::cout << nameAndItersStr;
  for(std::size_t ii = 70 - nameAndItersStr.length() - nanosStr.length(); ii; --ii) {
    std::cout << '.';
  }
  std::cout << nanosStr << " ns\n";
}

void promote::testing::printUnitResult(std::string const& name, 
                                       std::string const& error)
{
  bool const isError(error.length() != 0);
  std::string const status(isError ? "fail" : "pass");

  std::cout << name;
  for(std::size_t ii = 70 - name.length() - status.length(); ii; --ii) {
    std::cout << '.';
  }
  if(isError) {
    std::cout << "\033[22;31m" << status << "\n -> ERROR: " << error << "\033[0m\n";
  } else {
    std::cout << "\033[22;32m" << status << "\033[0m\n";
  }
}
