#ifndef PROMOTE_ARGUMENT_PARSER_HPP_
#define PROMOTE_ARGUMENT_PARSER_HPP_

#include <string>

#include "HashMap.hpp"
#include "HashSet.hpp"

namespace promote
{
    class ArgumentParser
    {
    public:
        ArgumentParser()  { }
        ~ArgumentParser() { }

        const HashSet<std::string>& flags() const { return _flags; }
        const HashMap<std::string, std::string>& parameters() const { return _parameters; }

        void process(const char* const* argv, unsigned argc);
    private:
        HashSet<std::string> _flags;
        HashMap<std::string, std::string> _parameters;
    };
}

#endif /* PROMOTE_ARGUMENT_PARSER_HPP_ */

