#include <cstring>

#include "ArgumentParser.hpp"

void 
promote::ArgumentParser::process(const char* const* argv, unsigned argc)
{
    unsigned ii(0);
    while( ii != argc ) {
        const std::string arg(argv[ii], strlen(argv[ii]));
        const bool hasDash(arg[0] == '-');
        if( hasDash ) {
            if( ii + 1 != argc and argv[ii+1][0] != '-' ) {
                const std::string param(argv[ii+1], strlen(argv[ii+1]));
                _parameters[arg] = param;
                //_parameters["foo" = "shoe";
                ii += 2;
            } else {
                _flags.add(&(arg[1]));
                ++ii;
            }
        } else {
            _flags.add(arg);
            ++ii;
        }
    }
}

