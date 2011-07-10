#ifndef PROMOTE_LINKABLE_HPP_
#define PROMOTE_LINKABLE_HPP_

#include "Array.hpp"
#include "Utilities.hpp"

namespace promote
{
    class BogusType { };

    template <typename T, typename LinkT=BogusType>
    class Linkable
    {
    public:
        typedef LinkT LinkType;

        Linkable(std::size_t const numLinks) 
        :   _links(numLinks)
        {
            for(std::size_t ii = 0; ii != numLinks; ++ii) {
                _links[ii] = NULL;
            }
        }
        Linkable(typename Passable<T const>::Type initItem,
                        std::size_t const numLinks)
        :   _item(initItem), 
            _links(numLinks)
        {
            for(std::size_t ii = 0; ii != numLinks; ++ii) {
                _links[ii] = NULL;
            }
        }

        /* Const */
        T const& item() const    { return _item; }
        LinkT const* link(std::size_t const index) const { return _links[index]; }

        /* Non-Const */
        T& item()    { return _item; }
        LinkT*& link(std::size_t const index) { return _links[index]; }
    private:
        T _item;
        Array<LinkT*> _links;
    };

    template <typename T>
    class Linkable<T,BogusType> : public Linkable<T, Linkable<T> >
    { 
    public:
        Linkable(std::size_t const numLinks)
        :   Linkable<T, Linkable<T> >(numLinks)
        { }
        Linkable(typename Passable<T const>::Type item,
                        std::size_t const numLinks)
        :   Linkable<T, Linkable<T> >(item, numLinks)
        { }
    };
    
    template <typename T, std::size_t L, typename LinkT=BogusType>
    class StaticLinkable
    {
    public:
        typedef LinkT LinkType;

        StaticLinkable() 
        :   _links(static_cast<LinkType*>(0)) 
        { }
        StaticLinkable(typename Passable<T const>::Type initItem) 
        :   _item(initItem)
        {
            for(std::size_t ii = 0; ii != L; ++ii) {
                _links[ii] = NULL;
            }
        }
        ~StaticLinkable() { }

        /* Const */
        T const& item() const    { return _item; }
        LinkT const* link(std::size_t const index) const { return _links[index]; }

        /* Non-Const */
        T& item()    { return _item; }
        LinkT*& link(std::size_t const index) { return _links[index]; }
    private:
        T _item;
        StaticArray<LinkT*,L> _links;
    };

    template <typename T, std::size_t L>
    class StaticLinkable<T,L,BogusType> : public StaticLinkable<T,L,StaticLinkable<T,L> >
    {
    public:
        typedef StaticLinkable<T,L> LinkType;

        StaticLinkable() 
        { }
        StaticLinkable(typename Passable<T const>::Type initItem) 
        :   StaticLinkable<T,L,StaticLinkable<T,L> >(initItem)
        { }

        /*T const& item() const { return _item; }
        LinkType const* link(std::size_t const index) const
        {
            return _links[index];
        }

        T& item() { return _item; }
        LinkType*& link(std::size_t const index)
        {
            return _links[index];
        }
    private:
        T _item;
        StaticArray<StaticLinkable<T,L>*,L> _links;*/
    };

    template <typename T, typename LinkT=BogusType>
    class SinglyLinkable : public StaticLinkable<T,1,LinkT>
    {
    public:
        SinglyLinkable() { }
        SinglyLinkable(typename Passable<T const>::Type initItem) : StaticLinkable<T,1,LinkT>(initItem) { }

        /* Const */
        typename StaticLinkable<T,1,LinkT>::LinkType const* next() const 
        {
            return StaticLinkable<T,1,LinkT>::link(0); 
        }

        /* Non-Const */
        typename StaticLinkable<T,1,LinkT>::LinkType*& next() 
        { 
            return StaticLinkable<T,1,LinkT>::link(0); 
        }
    };

    template <typename T>
    class SinglyLinkable<T,BogusType> : public SinglyLinkable<T,SinglyLinkable<T> >
    {
    public:
        SinglyLinkable() { }
        SinglyLinkable(typename Passable<T const>::Type item)
        :   SinglyLinkable<T,SinglyLinkable<T> >(item)
        { }

        /*SinglyLinkable<T> const* next() const
        {
            return StaticLinkable<T,1,SinglyLinkable<T> >::link(0);
        }
        
        SinglyLinkable<T>*& next()
        {
            return StaticLinkable<T,1,SinglyLinkable<T> >::link(0);
        }*/
    };
}

#endif /* PROMOTE_LINKABLE_HPP_ */

