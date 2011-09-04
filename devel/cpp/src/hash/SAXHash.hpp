#ifndef PROMOTE_SAX_HASH_HPP_
#define PROMOTE_SAX_HASH_HPP_

#include "Hash.hpp"

namespace promote
{
    hash_t hashSAX(const char* x, std::size_t len);

    template <typename T>
    class SAXHash : public Hash<T>
    {
    public:
        static const SAXHash<T>& instance()
        {
            static SAXHash<T> instance;
            return instance;
        }
        hash_t hash(const T& x) const
        {
            return hashSAX(reinterpret_cast<const char*>(x), sizeof(T));
        }
    };
}

#endif /* PROMOTE_SAX_HASH_HPP_ */

