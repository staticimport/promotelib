#ifndef PROMOTE_FNV_HASH_HPP_
#define PROMOTE_FNV_HASH_HPP_

#include "Hash.hpp"

namespace promote
{
    hash_t hashFNV(const char* x, std::size_t len);

    template <typename T>
    class FNVHash : public Hash<T>
    {
    public:
        static const FNVHash<T>& instance()
        {
            static FNVHash<T> instance;
            return instance;
        }
        hash_t hash(const T& x) const
        {
            return hashFNV(reinterpret_cast<const char*>(x), sizeof(T));
        }
    };
}

#endif /* PROMOTE_FNV_HASH_HPP_ */

