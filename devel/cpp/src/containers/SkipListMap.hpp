#ifndef PROMOTE_SKIP_LIST_MAP_HPP_
#define PROMOTE_SKIP_LIST_MAP_HPP_

#include "Allocator.hpp"
#include "Comparator.hpp"
#include "Exception.hpp"
#include "RandomGenerator.hpp"
#include "StandardAllocator.hpp"
#include "StandardRandomGenerator.hpp"

namespace promote
{
    template <typename T, bool USE_COMPARATOR=true>
    class SkipListMapComparator
    {
    public:
        SkipListMapComparator(Comparator<T> const& underlying)
        :   _underlying(underlying)
        { }

        ~SkipListMapComparator() { }
        
        bool isGreaterThan(T const& a, T const& b) const
        {
            return (_underlying.compare(a,b) == ORDER_DECREASING);
        }

        bool isEqual(T const& a, T const& b) const
        {
            return (_underlying.compare(a,b) == ORDER_SAME);
            //return (_lastOrder == ORDER_SAME);
        }

        bool isLessThan(T const& a, T const& b) const
        {
            _lastOrder = _underlying.compare(a,b);
            return (_lastOrder == ORDER_INCREASING);
        }
    private:
        Comparator<T> const& _underlying;
        Order _lastOrder;
    };

    template <typename T>
    class SkipListMapComparator<T,false>
    {
    public:
        SkipListMapComparator(Comparator<T> const& underlying) { }
        ~SkipListMapComparator() { }

        bool isGreaterThan(T const& a, T const& b) const
        {
            return (a > b);
        }

        bool isEqual(T const& a, T const& b) const
        {
            return a == b;
        }

        bool isLessThan(T const& a, T const& b) const
        {
            return (a < b);
        }
    };

    template <typename K, typename V>
    class SkipListMapEntry
    {
    public:
        typedef int level_t;

        SkipListMapEntry(K const& key,
                         SkipListMapEntry<K,V>** const forwards,
                         const level_t levels)
        :   _key(key), _forwards(forwards), _levels(levels)
        { }

        ~SkipListMapEntry() { }

        // Const
        K const& key() const                         { return _key; }
        SkipListMapEntry<K,V> const* const* forwards() const { return _forwards; }
        level_t levels() const                       { return _levels; }
        V const& value() const                       { return _value; }

        // Non-Const
        SkipListMapEntry<K,V>** forwards() { return _forwards; }
        V& value()                       { return _value; }
    private:
        K const _key;
        SkipListMapEntry<K,V>** const _forwards;
        const level_t _levels;
        V _value;
    };

    template <typename K, typename V, bool USE_COMPARATOR=IsClass<K>::Yes>
    class SkipListMap
    {
    private:
        //typedef promote::SkipListMapEntry<K,V>::level_t level_t;
        typedef int level_t; // TODO
        static const level_t MAX_LEVELS = sizeof(rand_t) << 3;
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        SkipListMap(Allocator& allocator = StandardAllocator::instance(),
                    Comparator<K> const& comparator = StandardComparator<K>::instance(),
                    RandomGenerator const& rand = StandardRandomGenerator::instance())
        :   _allocator(allocator),
            _comparator(comparator),
            _rand(rand),
            _size(0),
            _highestLevel(-1)
        {
            for(level_t l = 0; l < MAX_LEVELS; ++l) {
                _starts[l] = 0;
            }
        }

        ~SkipListMap();

        // Const
        bool contains(K const& key) const   { return (get(key) != 0); }
        bool empty() const                  { return (_size == 0); }
        bool find(K const& key, V const*& value) const
        {
            SkipListMapEntry<K,V> const* entry(get(key));
            if( entry ) {
                value = &(entry->value());
                return true;
            } else {
                return false;
            }
        }
        std::size_t size() const            { return _size; }
        V const& operator[](K const& key) const
        {
            SkipListMapEntry<K,V> const* entry(get(key));
            if( entry ) {
                return entry->value();
            } else {
                throw Exception("Unknown SkipListMap key!");
            }
        }

        // Non-Const
        bool find(K const& key, V*& value)
        {
            SkipListMapEntry<K,V>** forwards(_starts);
            SkipListMapEntry<K,V> *entry(0), *next(0);
            for(level_t l = _highestLevel; l != -1; --l) {
                entry = forwards[l];
                if( entry and !_comparator.isGreaterThan(entry->key(), key) ) {
                    while( ((next = entry->forwards()[l]) != 0) and
                            _comparator.isLessThan(next->key(), key) ) 
                    {
                        entry = next;
                    }
                    forwards = entry->forwards();
                }
            }
            return (entry and _comparator.isEqual(entry->key(), key));
        }
                
        V& operator[](K const& key)
        {
            SkipListMapEntry<K,V>** updates[MAX_LEVELS];
            SkipListMapEntry<K,V>* entry(0);
            level_t newHighestLevel(_highestLevel);
            if( _size ) {
                SkipListMapEntry<K,V>** forwards(_starts);
                SkipListMapEntry<K,V>* next(0);
                for(level_t l = _highestLevel; l != -1; --l) {
                    //printf("forwards[%d]=%p\n", l, forwards[l]);
                    if( forwards[l] and !_comparator.isGreaterThan(forwards[l]->key(), key) ) {
                        entry = forwards[l];
                  //      printf("level=%d, entry=%p below, next=%p\n", l, entry, entry->forwards()[l]);
                        while( ((next = entry->forwards()[l]) != 0) and
                                !_comparator.isGreaterThan(next->key(), key) )
                        {
                //            printf("set to next=%p\n", next);
                            entry = next;
                        }
                        updates[l] = forwards = entry->forwards();
                    } else {
                        updates[l] = forwards;
                    }
                }
            } else {
                //updates[_highestLevel = 0] = _starts;
                updates[newHighestLevel = 0] = _starts;
            }
            if( entry and _comparator.isEqual(entry->key(), key) ) {
                return entry->value();
            }

            // Randomly calculate levels of new entry
            level_t level(0);
            rand_t random = _rand.generate();
            while( level <= _highestLevel && level < MAX_LEVELS ) {
                if( random & 1 ) {
                    ++level;
                    random >>= 1;
                } else {
                    break;
                }
            }
            if( level > _highestLevel ) {
                updates[newHighestLevel = level] = _starts;
            }
            /*do {
                if( random & 1 ) {
                    if( level <= _highestLevel ) {
                        ++level;
                        random >>= 1;
                    } else {
                        updates[newHighestLevel = level] = _starts;
                        break;
                    }
                } else {
                    break;
                }
            } while( level != MAX_LEVELS );
            *///printf("level=%d\n", level);
            
            uint8_t* space = (uint8_t*)_allocator.alloc(sizeof(SkipListMapEntry<K,V>) + (sizeof(void*) * (level + 1)),false);
            SkipListMapEntry<K,V>** forwards = (SkipListMapEntry<K,V>**)(space + sizeof(SkipListMapEntry<K,V>));
            entry = new(space) SkipListMapEntry<K,V>(key, forwards, level);
            //printf("new entry=%p\n", entry);
            for(level_t l = level; l != -1; --l) {
                forwards[l] = updates[l][l];
                updates[l][l] = entry;
                //printf("Updated forwards[%d]=%p\n", l, forwards[l]);
            }
            _highestLevel = newHighestLevel;
            ++_size;
            //printf("starts[0]=%p, starts[1]=%p\n", _starts[0], _starts[1]);
            return entry->value();
        }
    private:
        SkipListMapEntry<K,V> const* get(K const& key) const
        {
            SkipListMapEntry<K,V>* const* forwards(_starts);
            SkipListMapEntry<K,V> *entry(0), *next(0);
            for(level_t l = _highestLevel; l != -1; --l) {
                if( forwards[l] and !_comparator.isGreaterThan(forwards[l]->key(), key) ) {
                    entry = forwards[l];
                    while( ((next = entry->forwards()[l]) != 0) and
                            !_comparator.isGreaterThan(next->key(), key) )
                    {
                        entry = next;
                    }
                    forwards = entry->forwards();
                }
            }
            if( entry and _comparator.isEqual(entry->key(), key) ) {
                return entry;
            } else {
                return 0;
            }
        }

        Allocator& _allocator;
        const SkipListMapComparator<K,USE_COMPARATOR> _comparator;
        RandomGenerator const& _rand;
        std::size_t _size;
        SkipListMapEntry<K,V>* _starts[MAX_LEVELS];
        level_t _highestLevel;
    };
}

/***
 * Implementation
 ***/
template <typename K, typename V, bool USE_COMPARATOR>
promote::SkipListMap<K,V,USE_COMPARATOR>::~SkipListMap()
{
    SkipListMapEntry<K,V>* entry = _starts[0];
    while( entry ){
        SkipListMapEntry<K,V>* next(entry->forwards()[0]);
        entry->~SkipListMapEntry<K,V>();
        _allocator.release(entry);
        entry = next;
    }
}

#endif /* PROMOTE_SKIP_LIST_MAP_HPP_ */

