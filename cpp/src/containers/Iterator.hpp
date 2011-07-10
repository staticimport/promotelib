#ifndef PROMOTE_ITERATOR_HPP_
#define PROMOTE_ITERATOR_HPP_

#include "Exception.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename T>
    class Iterator
    {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        virtual ~Iterator() { }
        virtual bool hasNext() = 0;
        virtual typename Passable<T>::Type next() = 0;
    };

    template <typename T>
    class EmptyIterator : public Iterator<T>
    {
    public:
        static EmptyIterator& instance()
        {
            static EmptyIterator instance;
            return instance;
        }

        bool hasNext() { return false; }
        typename Passable<T>::Type next()
        {
            throw Exception("EmptyConstIterator has no items.");
        }
    };

    template <typename INDEXABLE, typename T>
    class IndexableIterator : public Iterator<T>
    {
    public:
        IndexableIterator(INDEXABLE& indexable);
        IndexableIterator(INDEXABLE& indexable,
                          std::size_t length);
        virtual ~IndexableIterator() { }

        bool hasNext() { return _nextIndex != _length; }
        typename Passable<T>::Type next()
        {
            return _indexable[_nextIndex++];
        }
    private:
        INDEXABLE& _indexable;
        std::size_t const _length;
        std::size_t _nextIndex;
    };
}

/***
 * Implementation
 ***/
template <typename INDEXABLE, typename T>
promote::IndexableIterator<INDEXABLE,T>::IndexableIterator(INDEXABLE& indexable)
:   _indexable(indexable),
    _length(indexable.length()),
    _nextIndex(0)
{
}

template <typename INDEXABLE, typename T>
promote::IndexableIterator<INDEXABLE,T>::IndexableIterator(INDEXABLE& indexable,
                                                           std::size_t const length)
:   _indexable(indexable),
    _length(length),
    _nextIndex(0)
{
}

#endif /* PROMOTE_ITERATOR_HPP_ */
