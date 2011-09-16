#ifndef PROMOTE_TESTING_HPP
#define PROMOTE_TESTING_HPP

#include <string>

namespace promote {
  namespace testing {
    void printPerfResult(std::string const& name, double const nanos);
    void printPerfItersResult(std::string const& name,
                              std::size_t const iters,
                              double const nanos);
    void printUnitResult(std::string const& name, std::string const& error);
  };
}

#endif /* PROMOTE_TESTING_HPP */

