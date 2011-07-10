#include <string>

#include "FNVHash.hpp"
#include "System.hpp"

#ifdef __LP64__
static const promote::hash_t FNV_PRIME = 1099511628211ULL;
static const promote::hash_t FNV_OFFSET_BASIS = 0xcbf29ce484222325ULL;
#else
static const promote::hash_t FNV_PRIME = 16777619UL;
static const promote::hash_t FNV_OFFSET_BASIS = 0x811c9dc5UL;
#endif

promote::hash_t
promote::hashFNV(const char* x, std::size_t len)
{
    hash_t value(FNV_OFFSET_BASIS);
    for(std::size_t ii = 0; ii != len; ++ii) {
        value = FNV_PRIME * (value ^ x[ii]);
    }
    return value;
}

namespace promote
{
    template <> class FNVHash<std::string>
    {
    public:
        static const FNVHash<std::string>& instance()
        {
            static FNVHash<std::string> instance;
            return instance;
        }
        hash_t hash(const std::string& x) const
        {
            return hashFNV(x.c_str(), x.length());
        }
    };
}

