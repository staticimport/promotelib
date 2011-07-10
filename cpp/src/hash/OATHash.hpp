#ifndef PROMOTE_OAT_HASH_HPP_
#define PROMOTE_OAT_HASH_HPP_

#include "Hash.hpp"

namespace promote
{
    inline hash_t hashOAT(const char* x, std::size_t len);

    template <typename T>
    class OATHash : public Hash<T>
    {
    public:
        static const OATHash<T>& instance()
        {
            static OATHash<T> instance;
            return instance;
        }
        hash_t hash(const T& x) const
        {
            return hashOAT(reinterpret_cast<const char*>(x), sizeof(T));
        }
    };
}

#endif /* PROMOTE_OAT_HASH_HPP_ */

