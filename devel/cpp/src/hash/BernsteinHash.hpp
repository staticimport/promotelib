#ifndef PROMOTE_BERNSTEIN_HASH_HPP_
#define PROMOTE_BERNSTEIN_HASH_HPP_

#include "Hash.hpp"

namespace promote
{
    inline hash_t hashBernstein(const char* x, std::size_t len);

    template <typename T>
    class BernsteinHash : public Hash<T>
    {
    public:
        static const BernsteinHash<T>& instance()
        {
            static BernsteinHash<T> instance;
            return instance;
        }

        hash_t hash(T const& x) const
        {
            return hashBernstein(reinterpret_cast<const char*>(x), sizeof(T));
        }
    };
}

#endif /* PROMOTE_STANDARD_HASH_HPP_ */

