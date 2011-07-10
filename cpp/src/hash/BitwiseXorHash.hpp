#ifndef PROMOTE_STANDARD_HASH_HPP_
#define PROMOTE_STANDARD_HASH_HPP_

#include "Hash.hpp"

namespace promote
{
    template <typename T, bool Method=IsClass<T>::Yes>
    class StandardHash : public Hash<T>
    {
    public:
        static const StandardHash<T,Method>& instance()
        {
            static StandardHash<T,Method> instance;
            return instance;
        }

        hash_t hash(const T& x) const
        {
            return x.hashcode();
        }
    };

    template <typename T>
    class StandardHash<T,false> : public Hash<T>
    {
    public:
        static const StandardHash<T,false>& instance()
        {
            static StandardHash<T,false> instance;
            return instance;
        }
        
        hash_t hash(const T& x) const
        {
            return static_cast<hash_t>(x);
        }
    };
}

#endif /* PROMOTE_STANDARD_HASH_HPP_ */

