#include <string>

#include "SAXHash.hpp"

promote::hash_t
promote::hashSAX(const char* x, std::size_t len)
{
    hash_t value(0);
    for(std::size_t ii = 0; ii != len; ++ii) {
        value ^= (value << 5) + (value >> 2) + x[ii];
    }
    return value;
}

namespace promote
{
    template <> class SAXHash<std::string>
    {
    public:
        static const SAXHash<std::string>& instance()
        {
            static SAXHash<std::string> instance;
            return instance;
        }
        hash_t hash(const std::string& x) const
        {
            return hashSAX(x.c_str(), x.length());
        }
    };
}

