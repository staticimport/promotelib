#include <string>

#include "OATHash.hpp"

inline promote::hash_t
promote::hashOAT(const char* x, std::size_t len)
{
    hash_t value(0);
    for(std::size_t ii = 0; ii != len; ++ii) {
        value += ((value + x[ii]) << 10);
        value ^= value >> 6;
    }
    value += (value << 3);
    value ^= (value >> 11);
    value += (value << 15);
    return value;
}

namespace promote
{
    template <> class OATHash<std::string>
    {
    public:
        static const OATHash<std::string>& instance()
        {
            static OATHash<std::string> instance;
            return instance;
        }
        hash_t hash(const std::string& x) const
        {
            return hashOAT(x.c_str(), x.length());
        }
    };
}

