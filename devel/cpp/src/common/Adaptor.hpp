#ifndef PROMOTE_ADAPTOR_HPP_
#define PROMOTE_ADAPTOR_HPP_

namespace promote
{
    template <typename I, typename O>
    class Adaptor
    {
    public:
        virtual ~Adaptor() { }
        virtual O convert(I const& in) const = 0;
    };

    template <typename I, typename O>
    class ConstCastAdaptor : public Adaptor<I,O>
    {
    public:
        static ConstCastAdaptor<I,O> const& instance()
        {
            static ConstCastAdaptor<I,O> instance;
            return instance;
        }

        O convert(I const& in) const
        {
            return const_cast<O>(in);
        }
    private:
        ~ConstCastAdaptor() { }
    };

    template <typename T>
    class IdentityAdaptor : public Adaptor<T,T>
    {
    public:
        static IdentityAdaptor<T> const& instance()
        {
            static IdentityAdaptor<T> instance;
            return instance;
        }

        T convert(T const& in) const
        {
            return in;
        }
    private:
        ~IdentityAdaptor() { }
    };

    template <typename I, typename O>
    class StaticCastAdaptor : public Adaptor<I,O>
    {
    public:
        static StaticCastAdaptor<I,O> const& instance()
        {
            static StaticCastAdaptor<I,O> instance;
            return instance;
        }

        O convert(I const& in) const
        {
            return static_cast<O>(in);
        }
    private:
        ~StaticCastAdaptor() { }
    };
}

#endif /* PROMOTE_ADAPTOR_HPP_ */

