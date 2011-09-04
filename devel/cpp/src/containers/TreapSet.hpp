#ifndef PROMOTE_TREAP_SET_HPP_
#define PROMOTE_TREAP_SET_HPP_

#include <cstdlib>
#include <iostream>
#include <stack>

#include "Allocator.hpp"
#include "Comparator.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "RandomGenerator.hpp"
#include "Stack.hpp"
#include "StandardAllocator.hpp"
#include "StandardRandomGenerator.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename T>
    class TreapSetEntry
    {
    public:
        TreapSetEntry(typename Passable<T>::Type const item, 
                      promote::rand_t heapKey, 
                      TreapSetEntry<T>* parent)
        :   _item(item), _heapKey(heapKey), _parent(parent), _left(0), _right(0)
        { }

        ~TreapSetEntry() { }

        T const& item() const   { return _item; }
        T& item()               { return _item; }

        void rotateLeft(void)
        {
            TreapSetEntry<T>* oldParent(_parent);
            if( (_right = (_parent = _right)->_left) ) {
                _right->_parent = this;
            }
            _parent->_left = this;
            if( (_parent->_parent = oldParent) ) {
                if( oldParent->_left == this ) {
                    oldParent->_left = _parent;
                } else {
                    oldParent->_right = _parent;
                }
            }
        }

        void rotateRight(void)
        {
            TreapSetEntry<T>* oldParent(_parent);
            if( (_left = (_parent = _left)->_right) ) {
                _left->_parent = this;
            }
            _parent->_right = this;
            if( (_parent->_parent = oldParent) ) {
                if( oldParent->_left == this ) {
                    oldParent->_left = _parent;
                } else {
                    oldParent->_right = _parent;
                }
            }
        }

        typename Passable<T>::Type const _item;
        promote::rand_t _heapKey;
        TreapSetEntry<T> *_parent, *_left, *_right;
    };
        /**
         * Iterators
         **/
        template <typename E>
        class TreapSetIterator : public promote::Iterator<E>
        {
        public:
            TreapSetIterator() { }
            ~TreapSetIterator()
            {
                _stack.clear();
            }

            bool hasNext()  { return !_stack.empty(); }
            typename Passable<E>::Type next()
            {
                while( _stack.top().second ) {
                    E* entry(_stack.top().first);
                    _stack.pop();
                    if( entry->_right ) {
                        std::pair<E*,bool> right(entry->_right,true);
                        _stack.push(right);
                    }
                    std::pair<E*,bool> mid(entry,false);
                    _stack.push(mid);
                    if( entry->_left ) {
                        std::pair<E*,bool> left(entry->_left,true);
                        _stack.push(left);
                    }
                }

                E* entry(_stack.top().first);
                _stack.pop();
                return *entry;
            }
            void initialize(E* root)
            {
                _stack.clear();
                if( root ) {
                    std::pair<E*,bool> pair(root,true);
                    _stack.push(pair);
                }
            }
        private:
            Stack<std::pair<E*,bool> > _stack;
        };

    template <typename T, bool USE_COMPARATOR=IsClass<T>::Yes>
    class TreapSet
    {
    public:
        static const std::size_t ALLOC_SIZE = sizeof(TreapSetEntry<T>);

        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        typedef TreapSetIterator<const TreapSetEntry<T> > ConstIterator;
        typedef TreapSetIterator<TreapSetEntry<T> > Iterator;

        TreapSet(Allocator& allocator=StandardAllocator::instance(),
                 const RandomGenerator& rand=StandardRandomGenerator::instance())
        :   _allocator(allocator), _rand(rand), _size(0), _root(0)
        { }

        ~TreapSet()
        {
            if( _root ) {
                std::stack<TreapSetEntry<T>*> entries;
                entries.push(_root);
                do {
                    TreapSetEntry<T>* entry(entries.top());
                    entries.pop();
                    if( entry->_left ) {
                        entries.push(entry->_left);
                    }
                    if( entry->_right ) {
                        entries.push(entry->_right);
                    }
                    _allocator.release(entry);
                } while( !entries.empty() );
            }
        }

        /**
         * Observe
         **/
        bool contains(typename Passable<T>::Type const item) const
        {
            TreapSetEntry<T>* entry(_root);
            while( entry ) {
                typename Passable<T>::Type const entryItem(entry->_item);
                if( item < entryItem ) {
                    entry = entry->_left;
                } else if( !(item == entryItem) ) {
                    entry = entry->_right;
                } else {
                    /*rand_t const newHeapKey(_rand.generate());
                    if( newHeapKey < entry->_heapKey ) {
                        entry->_heapKey = newHeapKey;
                        rotateUp(entry);
                    }*/
                    return true;
                }
            }
            return false;
        }
        bool empty() const { return (_size == 0); }
        void iterator(ConstIterator& iter) const
        {
            iter.initialize(_root);
        }
        std::size_t size() const { return _size; }

        /**
         * Modify
         **/
        bool add(typename Passable<T>::Type const item)
        {
            TreapSetEntry<T>* entry(_root);
            TreapSetEntry<T>* parent(0);
            bool isLeft(false);
            while( entry ) {
                typename Passable<T>::Type const entryItem(entry->_item);
             //   std::cout << item << " vs. " << entryItem << std::endl;
                if( item < entryItem ) {
                    entry = (parent = entry)->_left;
                    isLeft = true;
                    //break;
                } else if( item != entryItem ) {
                    entry = (parent = entry)->_right;
                    isLeft = false;
                    //break;
                } else {
                    return false;
                }
            }

            // New Entry
            void* space = _allocator.alloc(ALLOC_SIZE, false);
            if( space == 0 ) {
                throw Exception("Allocator failed.");
            }
            const rand_t heapKey(_rand.generate());
            entry = new(space) TreapSetEntry<T>(item, heapKey, parent);
            ++_size;
            if( parent ) {
                (isLeft ? parent->_left : parent->_right) = entry;
                // Rotate up according to heapKeys
                rotateUp(entry);
                /*do {
                    if( heapKey < parent->_heapKey ) {
                        if( parent->_left == entry ) {
                            parent->rotateRight();
                        } else {
                            parent->rotateLeft();
                        }
                        parent = entry->_parent;
                    } else {
                        return true;
                        //break;
                    }
                } while( parent );*/
            } else {
                _root = entry;
            }
            //++_size;
            return true;
        }
        void iterator(Iterator& iter)
        {
            iter.initialize(_root);
        }
    private:
        void rotateUp(TreapSetEntry<T>* const entry)
        {
            rand_t const heapKey(entry->_heapKey);
            for(TreapSetEntry<T>* parent = entry->_parent; parent; parent = entry->_parent) {
                if( heapKey < parent->_heapKey ) {
                    if( parent->_left == entry ) {
                        parent->rotateRight();
                    } else {
                        parent->rotateLeft();
                    }
                } else {
                    break;
                }
            }
            if( entry->_parent == 0 ) {
                _root = entry;
            }
        }

        Allocator& _allocator;
        const RandomGenerator& _rand;
        std::size_t _size;
        TreapSetEntry<T>* _root;
    };

    template <typename T>
    class TreapSet<T,true>
    {
    public:
        static const std::size_t ALLOC_SIZE = sizeof(TreapSetEntry<T>);
        
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };
        
        typedef TreapSetIterator<const TreapSetEntry<T> > ConstIterator;
        typedef TreapSetIterator<TreapSetEntry<T> > Iterator;

        TreapSet(Allocator& allocator=StandardAllocator::instance(),
                 const Comparator<T>& comparator=StandardComparator<T>::instance(),
                 const RandomGenerator& rand=StandardRandomGenerator::instance())
        :   _allocator(allocator), _comparator(comparator),
            _rand(rand), _size(0), _root(0)
        { }

        ~TreapSet()
        {
            if( _root ) {
                std::stack<TreapSetEntry<T>*> entries;
                entries.push(_root);
                do {
                    TreapSetEntry<T>* entry(entries.top());
                    entries.pop();
                    if( entry->_left ) {
                        entries.push(entry->_left);
                    }
                    if( entry->_right ) {
                        entries.push(entry->_right);
                    }
                    _allocator.release(entry);
                } while( !entries.empty() );
            }
        }

        /**
         * Observe
         **/
        bool contains(typename Passable<T>::Type const item) const
        {
            TreapSetEntry<T>* entry(_root);
            while( entry ) {
                switch(_comparator.compare(item, entry->_item))
                {
                    case ORDER_INCREASING:
                        entry = entry->_left;
                        break;
                    case ORDER_DECREASING:
                        entry = entry->_right;
                        break;
                    case ORDER_SAME:
                        rand_t const newHeapKey(_rand.generate());
                        if( newHeapKey < entry->_heapKey ) {
                            entry->_heapKey = newHeapKey;
                            rotateUp(entry);
                        }
                        return true;
                }
            }
            return false;
        }
        bool empty() const { return (_size == 0); }
        void iterator(ConstIterator& iter) const
        {
            iter.initialize(_root);
        }
        std::size_t size() const { return _size; }

        /**
         * Modify
         **/
        bool add(typename Passable<T>::Type const item)
        {
            TreapSetEntry<T>* entry(_root);
            TreapSetEntry<T>* parent(0);
            bool isLeft(false);
            while( entry ) {
                /*if( item < entry->_item ) {
                    entry = (parent = entry)->_left;
                    isLeft = true;
                    break;
                } else if( item != entry->_item ) {
                    entry = (parent = entry)->_right;
                    isLeft = true;
                    break;
                } else {
                    return false;
                }*/
                
                switch(_comparator.compare(item, entry->_item))
                {
                    case ORDER_INCREASING:
                        entry = (parent = entry)->_left;
                        isLeft = true;
                        break;
                    case ORDER_DECREASING:
                        entry = (parent = entry)->_right;
                        isLeft = false;
                        break;
                    case ORDER_SAME:
                        return false;
                }
            }

            // New Entry
            void* space = _allocator.alloc(ALLOC_SIZE, false);
            if( space == 0 ) {
                throw Exception("Allocator failed.");
            }
            const rand_t heapKey(_rand.generate());
            entry = new(space) TreapSetEntry<T>(item, heapKey, parent);
            ++_size;
            if( parent ) {
                (isLeft ? parent->_left : parent->_right) = entry;
                // Rotate up according to heapKeys
                rotateUp(entry);
                /*do {
                    if( heapKey < parent->_heapKey ) {
                        if( parent->_left == entry ) {
                            parent->rotateRight();
                        } else {
                            parent->rotateLeft();
                        }
                        parent = entry->_parent;
                    } else {
                        return true;
                    }
                } while( parent );*/
            } else {
                _root = entry;
            }
            return true;
        }
        void iterator(Iterator& iter)
        {
            iter.initialize(_root);
        }
    private:
        void rotateUp(TreapSetEntry<T>* const entry)
        {
            rand_t const heapKey(entry->_heapKey);
            for(TreapSetEntry<T>* parent = entry->_parent; parent; parent = entry->_parent) {
                if( heapKey < parent->_heapKey ) {
                    if( parent->_left == entry ) {
                        parent->rotateRight();
                    } else {
                        parent->rotateLeft();
                    }
                } else {
                    break;
                }
            }
            if( entry->_parent == 0 ) {
                _root = entry;
            }
        }

        Allocator& _allocator;
        const Comparator<T>& _comparator;
        const RandomGenerator& _rand;
        std::size_t _size;
        TreapSetEntry<T>* _root;
    };
}

#endif /* PROMOTE_TREAP_SET_HPP_ */

