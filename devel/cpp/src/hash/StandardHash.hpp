#ifndef PROMOTE_STANDARD_HASH_HPP_
#define PROMOTE_STANDARD_HASH_HPP_

#include "FNVHash.hpp"
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

        hash_t hash(T const& x) const
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
        
        hash_t hash(T const& x) const
        {
            //return static_cast<hash_t>(x);
            return (hash_t)x;
        }
    };

    template <bool Method>
    class StandardHash<std::string, Method> : public Hash<std::string>
    {
    public:
        static const StandardHash<std::string,Method>& instance()
        {
            static StandardHash<std::string,Method> instance;
            return instance;
        }

        hash_t hash(const std::string& x) const
        {
            return hashFNV(x.c_str(), x.length());
        }
    };

    template <typename T>
    class StandardReferenceHash : public StandardHash<T&, IsClass<T>::Yes>
    {
    public:
        static const StandardReferenceHash<T>& instance()
        {
            static StandardReferenceHash<T> instance;
            return instance;
        }
    };

/*    template <bool Method>
    class StandardHash<promote::String, Method> : public Hash<promote::String>
    {
    public:
        static const StandardHash<promote::String,Method>& instance()
        {
            static StandardHash<promote::String,Method> instance;
            return instance;
        }

        hash_t hash(const std::string& x
    };*/
}

#endif /* PROMOTE_STANDARD_HASH_HPP_ */

