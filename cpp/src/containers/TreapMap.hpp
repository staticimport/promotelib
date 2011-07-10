#ifndef PROMOTE_TREAP_MAP_HPP_
#define PROMOTE_TREAP_MAP_HPP_

#include <cstdlib>
#include <iostream>

#include "Allocator.hpp"
#include "Comparator.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "Exception.hpp"
#include "RandomGenerator.hpp"
#include "Stack.hpp"
#include "StandardAllocator.hpp"
#include "StandardRandomGenerator.hpp"
#include "TreeMap.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename K, typename V>
    class TreapMapEntry
    {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        TreapMapEntry(typename Passable<K const>::Type key, rand_t heapKey);
        ~TreapMapEntry() { }

        typename Passable<K const>::Type key() const    { return _key; }
        V const& value() const { return _value; }
        V& value() { return _value; }
        //typename Passable<V const>::Type value() const  { return _value; }
        //typename Passable<V>::Type value()              { return _value; }

        rand_t heapKey() const { return _heapKey; }

        TreapMapEntry<K,V>* left() const      { return _left; }
        TreapMapEntry<K,V>* parent() const    { return _parent; }
        TreapMapEntry<K,V>* right() const     { return _right; }

        TreapMapEntry<K,V>*& left()       { return _left; }
        TreapMapEntry<K,V>*& parent()     { return _parent; }
        TreapMapEntry<K,V>*& right()      { return _right; }
        
        void rotateLeft(void)
        {
            TreapMapEntry<K,V>* oldParent(_parent);
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
            TreapMapEntry<K,V>* oldParent(_parent);
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
        
    private:
        const K _key;
        const rand_t _heapKey;
        V _value;
        TreapMapEntry<K,V> *_left, *_right, *_parent;
    };

    template <typename E>
    class TreapMapIterator : public Iterator<E>
    {
    public:
        TreapMapIterator();
        /*TreapMapIterator()
        :   _next(0), _stack(32)
        { }*/

        bool hasNext()  { return prepNext(); }
        typename Passable<E>::Type next()
        {
            prepNext();
            E& next(*_next);
            _next = 0;
            return next;
        }
    //private:
        void initialize(E* root)
        {
            while(!_stack.empty()) {
                _stack.pop();
            }
            if( root ) {
                _stack.push(IteratorStep(root));
            }
        }
    private:
        struct IteratorStep
        {
            enum State { NEW, LEFT_DONE, SELF_DONE };

            IteratorStep(E* theEntry, State theState = NEW)
            :   entry(theEntry), state(theState)
            { }
            IteratorStep(IteratorStep const& is)
            :   entry(is.entry), state(is.state)
            { }

            E* entry;
            State state;
        };

        bool prepNext()
        {
            if( _next != 0 ) return true;

            while(!_stack.empty()) {
                IteratorStep& step(_stack.peek());
                switch(step.state) {
                    case IteratorStep::NEW: 
                        if( step.entry->left() ) {
                            step.state = IteratorStep::LEFT_DONE;
                            _stack.push(IteratorStep(step.entry->left()));
                            continue;
                        } // Else fall through...
                    
                    case IteratorStep::LEFT_DONE: {
                        step.state = IteratorStep::SELF_DONE;
                        _next = step.entry;
                        return true;
                    }
                    case IteratorStep::SELF_DONE: {
                        E* const right(step.entry->right());
                        _stack.pop();
                        if( right ) {
                            _stack.push(IteratorStep(right));
                        }
                        continue;
                    }
                }
            }
            return false; 
        }


        E* _next;
        Stack<IteratorStep> _stack;
    };

    template <typename K, typename V, bool USE_COMPARATOR=IsClass<K>::Yes>
    class TreapMap
    {
    public:
        static const std::size_t ALLOC_SIZE = sizeof(TreapMapEntry<K,V>);

        typedef TreapMapIterator<TreapMapEntry<K,V> > Iterator;
        typedef TreapMapIterator<TreapMapEntry<K,V> const > ConstIterator;

        TreapMap(Allocator& allocator=StandardAllocator::instance(),
                 Comparator<K> const& comparator = StandardComparator<K>::instance(),
                 RandomGenerator const& rand = StandardRandomGenerator::instance())
        :   _allocator(allocator),
            _comparator(comparator),
            _rand(rand),
            _size(0),
            _root(0)
        { }

        ~TreapMap() { clear(); }

        /* Const */
        bool contains(typename Passable<K const>::Type key) const { return (get(key) != 0); }
        bool empty() const                  { return (_size == 0); }
        bool find(typename Passable<K const>::Type key, V const*& value) const
        {
            TreapMapEntry<K,V> const* entry(get(key));
            if( entry ) {
                value = &(entry->value());
                return true;
            } else {
                return false;
            }
        }
        void iterator(ConstIterator& iter) const
        {
            iter.initialize(_root);
        }
        std::size_t size() const            { return _size; }
        //typename Passable<V const>::Type operator[](typename Passable<K const>::Type key) const
        V const& operator[](typename Passable<K const>::Type key) const
        {
            TreapMapEntry<K,V> const* entry(get(key));
            if( entry ) {
                return entry->value();
            } else {
                throw Exception("Unknown TreapMap key!");
            }
        }

        /* Non-Const */
        void clear()
        {
            if( _root ) {
                Stack<TreapMapEntry<K,V>*> stack(16);
                stack.push(_root);
                do {
                    TreapMapEntry<K,V>* entry(stack.peek());
                    stack.pop();
                    if( entry->left() ) {
                        stack.push(entry->left());
                    }
                    if( entry->right() ) {
                        stack.push(entry->right());
                    }
                    _allocator.release(entry);
                } while( !stack.empty() );
            }
            _root = 0;
            _size = 0;
        }
        bool find(typename Passable<K const>::Type key, V*& value)
        {
            TreapMapEntry<K,V>* entry(_root);
            while( entry ) {
                switch(_comparator.compare(key, entry->key()))
                {
                    case ORDER_INCREASING:
                        //entry = dynamic_cast<TreapMapEntry<K,V>*>(entry->left());
                        entry = entry->left();
                        break;
                    case ORDER_DECREASING:
                        //entry = dynamic_cast<TreapMapEntry<K,V>*>(entry->right());
                        entry = entry->right();
                        break;
                    case ORDER_SAME:
                        value = &(entry->value());
                        return true;
                }
            }
            return false;
        }
        void iterator(Iterator& iter)
        {
            iter.initialize(_root);
        }
        //typename Passable<V>::Type operator[](typename Passable<K const>::Type key)
        V& operator[](typename Passable<K const>::Type key)
        {
            // Lookup key
            TreapMapEntry<K,V>* entry(_root);
            TreapMapEntry<K,V>* parent(0);
            Order order(ORDER_SAME);
            while(entry) {
                parent = entry;
                switch(order = _comparator.compare(key, entry->key()))
                {
                    case ORDER_INCREASING:
                        entry = entry->left();
                        break;
                    case ORDER_DECREASING:
                        entry = entry->right();
                        break;
                    case ORDER_SAME:
                        return entry->value();
                }
            }

            // New Entry
            void* space(_allocator.alloc(ALLOC_SIZE, false));
            if( space == 0 ) {
                throw Exception("TreapMap Allocator failed.");
            }
            const rand_t heapKey(_rand.generate());
            entry = new(space) TreapMapEntry<K,V>(key, heapKey);
            ++_size;
            if( parent ) {
                // Insert entry below parent
                entry->parent() = parent;
                if( order == ORDER_INCREASING ) {
                    //static_cast<TreeEntry*>(parent)->left(entry);
                    parent->left() = entry;
                } else {
                    //static_cast<TreeEntry*>(parent)->right(entry);
                    parent->right() = entry;
                }

                // Rotate up according to heap keys
                do {
                    if( heapKey < parent->heapKey() ) {
                        if( parent->left() == entry ) {
                            parent->rotateRight();
                        } else {
                            parent->rotateLeft();
                        }
                        parent = entry->parent();
                    } else {
                        return entry->value();
                    }
                } while( parent );
            }
            _root = entry;
            return entry->value();
        }

    private:
        TreapMapEntry<K,V> const* get(typename Passable<K const>::Type key) const
        {
            TreapMapEntry<K,V> const* entry(_root);
            while( entry ) {
                switch(_comparator.compare(key, entry->key()))
                {
                    case ORDER_INCREASING:
                        entry = entry->left();
                        break;
                    case ORDER_DECREASING:
                        entry = entry->right();
                        break;
                    case ORDER_SAME:
                        return entry;
                }
            }
            return 0;
        }

        Allocator& _allocator;
        Comparator<K> const& _comparator;
        RandomGenerator const& _rand;
        std::size_t _size;
        TreapMapEntry<K,V>* _root;
    };

    template <typename K, typename V>
    class TreapMap<K,V,false>
    {
    public:
        static const std::size_t ALLOC_SIZE = sizeof(TreapMapEntry<K,V>);

        typedef TreapMapIterator<TreapMapEntry<K,V> > Iterator;
        typedef TreapMapIterator<TreapMapEntry<K,V> const > ConstIterator;

        TreapMap(Allocator& allocator=StandardAllocator::instance(),
                 RandomGenerator const& rand = StandardRandomGenerator::instance())
        :   _allocator(allocator),
            _rand(rand),
            _size(0),
            _root(0)
        { }

        ~TreapMap() { clear(); }

        /* Const */
        bool contains(typename Passable<K const>::Type key) const   { return (get(key) != 0); }
        bool empty() const                  { return (_size == 0); }
        bool find(typename Passable<K const>::Type key, V const*& value) const
        {
            TreapMapEntry<K,V> const* entry(get(key));
            if( entry ) {
                value = &(entry->value());
                return true;
            } else {
                return false;
            }
        }
        void iterator(ConstIterator& iter) const
        {
            iter.initialize(_root);
        }
        std::size_t size() const            { return _size; }
        //typename Passable<V const>::Type  operator[](typename Passable<K const>::Type key) const
        V const& operator[](typename Passable<K const>::Type key) const
        {
            TreapMapEntry<K,V> const* entry(get(key));
            if( entry ) {
                return entry->value();
            } else {
                throw Exception("Unknown TreapMap key!");
            }
        }

        /* Non-Const */
        void clear()
        {
            if( _root ) {
                Stack<TreapMapEntry<K,V>*> stack(16);
                stack.push(_root);
                do {
                    TreapMapEntry<K,V>* entry(stack.peek());
                    stack.pop();
                    if( entry->left() ) {
                        stack.push(entry->left());
                    }
                    if( entry->right() ) {
                        stack.push(entry->right());
                    }
                    _allocator.release(entry);
                } while( !stack.empty() );
            }
            _root = 0;
            _size = 0;
        }

        bool find(typename Passable<K const>::Type key, V*& value)
        {
            TreapMapEntry<K,V>* entry(_root);
            while( entry ) {
                if( key < entry->key() ) {
                    entry = entry->left();
                } else if( key != entry->key() ) {
                    entry = entry->right();
                } else {
                    value = &(entry->value());
                    return true;
                }
            }
            return false;
        }
        void iterator(Iterator& iter)
        {
            iter.initialize(_root);
        }
        //typename Passable<V>::Type operator[](typename Passable<K const>::Type key)
        V& operator[](typename Passable<K const>::Type key)
        {
            // Lookup key
            TreapMapEntry<K,V>* entry(_root);
            TreapMapEntry<K,V>* parent(0);
            Order order(ORDER_SAME);
            while(entry) {
                parent = entry;
                if( key < entry->key() ) {
                    order = ORDER_INCREASING;
                    entry = entry->left();
                } else if( key != entry->key() ) {
                    order = ORDER_DECREASING;
                    entry = entry->right();
                } else {
                    return entry->value();
                }
            }

            // New Entry
            void* space(_allocator.alloc(ALLOC_SIZE, false));
            if( space == 0 ) {
                throw Exception("TreapMap Allocator failed.");
            }
            const rand_t heapKey(_rand.generate());
            entry = new(space) TreapMapEntry<K,V>(key, heapKey);
            ++_size;
            if( parent ) {
                // Insert entry below parent
                entry->parent() = parent;
                if( order == ORDER_INCREASING ) {
                    //static_cast<TreeEntry*>(parent)->left(entry);
                    parent->left() = entry;
                } else {
                    //static_cast<TreeEntry*>(parent)->right(entry);
                    parent->right() = entry;
                }

                // Rotate up according to heap keys
                do {
                    if( heapKey < parent->heapKey() ) {
                        if( parent->left() == entry ) {
                            parent->rotateRight();
                        } else {
                            parent->rotateLeft();
                        }
                        parent = entry->parent();
                    } else {
                        return entry->value();
                    }
                } while( parent );
            }
            _root = entry;
            return entry->value();
        }

    private:
        TreapMapEntry<K,V> const* get(typename Passable<K const>::Type key) const
        {
            TreapMapEntry<K,V> const* entry(_root);
            while( entry ) {
                if( key < entry->key() ) {
                    entry = entry->left();
                } else if( key != entry->key() ) {
                    entry = entry->right();
                } else {
                    return entry;
                }
            }
            return 0;
        }

        Allocator& _allocator;
        RandomGenerator const& _rand;
        std::size_t _size;
        TreapMapEntry<K,V>* _root;
    };
}

/***
 * Implementation
 ***/
template <typename K, typename V>
promote::TreapMapEntry<K,V>::TreapMapEntry(typename Passable<K const>::Type key, rand_t heapKey)
/*:   promote::TreeMapEntry<K,V>(key),
    _heapKey(heapKey)*/
:   _key(key), _heapKey(heapKey), _left(0), _right(0), _parent(0)
{
}

template <typename E>
promote::TreapMapIterator<E>::TreapMapIterator()
:   _next(0), _stack(32)
{
}

/*template <typename K, typename V, bool USE_COMPARATOR>
promote::TreapMap<K,V,USE_COMPARATOR>::TreapMap(Allocator& allocator,
                                                Comparator<K> const& comparator,
                                                RandomGenerator const& rand)
:   _allocator(allocator),
    _comparator(comparator),
    _rand(rand),
    _size(0),
    _root(0)
{
}*/

#endif /* PROMOTE_TREAP_MAP_HPP_ */

