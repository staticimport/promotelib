#include <string>

#include "BernsteinHash.hpp"

inline promote::hash_t
promote::hashBernstein(const char* x, std::size_t len)
{
    hash_t value(0);
    for(std::size_t ii = 0; ii != len; ++ii) {
        value = ((value << 5) + value) ^ x[ii];
    }
    return value;
}

namespace promote
{
    template <> class BernsteinHash<std::string>
    {
    public:
        static const BernsteinHash<std::string>& instance() 
        {
            static BernsteinHash<std::string> instance;
            return instance;
        }
        hash_t hash(const std::string& x) const
        {
            return hashBernstein(x.c_str(), x.length());
        }
    };
}

