#ifndef PROMOTE_HASH_MAP_HPP_
#define PROMOTE_HASH_MAP_HPP_

#include <cstdlib>
#include <cmath>

#include "Allocator.hpp"
#include "Exception.hpp"
#include "Hash.hpp"
#include "Initializer.hpp"
#include "Iterator.hpp"
#include "Power.hpp"
#include "StandardAllocator.hpp"
#include "StandardHash.hpp"

namespace promote
{
    const float DEFAULT_HASH_MAP_LOAD_FACTOR = 0.6;

    template <typename K, typename V>
    class HashMap
    {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        template <typename E> class HashMapIterator;

        /**
         * Entry
         **/
        class Entry
        {
        public:
            ~Entry() { }

            K const& key() const        { return _key; }
            K const* keyPtr() const     { return &_key; }
            V& value()                  { return _value; }
            const V& value() const      { return _value; }
            V* valuePtr()               { return &_value; }
            const V* valuePtr() const   { return &_value; }
        private:
            Entry(K const& key, hash_t hashcode, Entry* next = 0)
            : _key(key), _hashcode(hashcode), _next(next)
            { }

            hash_t hashcode() const { return _hashcode; }

            Entry const* next() const { return _next; }
            Entry* next() { return _next; }
            void next(Entry* const next) { _next = next; }

            //friend class ConstIterator;
            friend class HashMap<K,V>;
            friend class HashMapIterator<const Entry>;
            friend class HashMapIterator<Entry>;

            K const _key;
            const hash_t _hashcode;
            V _value;
            Entry* _next;
        };

        /**
         * Iterators
         **/
        template <typename E>
        class HashMapIterator : public promote::Iterator<E>
        {
        public:
            bool hasNext()  { return _nextEntry != 0; }
            typename Passable<E>::Type next()
            {
                E& entry(*_nextEntry);
                prepNextEntry();
                return entry;
            }
        private:
            void initialize(E* const* buckets, std::size_t bucketCount)
            {
                _nextEntry = 0;
                _buckets = buckets;
                _bucketCount = bucketCount;
                _nextBucket = 0;
                prepNextEntry();
            }

            void prepNextEntry()
            {
                // Walk current entry chain
                if( _nextEntry ) {
                    _nextEntry = _nextEntry->next();
                    if( _nextEntry ) {
                        return;
                    }
                }
                
                // Find next entry chain in buckets
                while( _nextBucket != _bucketCount ) {
                    _nextEntry = _buckets[_nextBucket++];
                    if( _nextEntry ) {
                        return;
                    }
                }
                
                // No luck
                _nextEntry = 0;
            }

            friend class HashMap<K,V>;

            E* _nextEntry;
            E* const* _buckets;
            std::size_t _bucketCount;
            std::size_t _nextBucket;
        };

        typedef HashMapIterator<Entry const> ConstIterator;
        typedef HashMapIterator<Entry> Iterator;

        static const std::size_t ALLOC_SIZE = sizeof(Entry);

        HashMap(std::size_t initCapacity = 16,
                Allocator& allocator=promote::StandardAllocator::instance(),
                Initializer<V>& initializer=StandardInitializer<V>::instance(),
                const Hash<K>& hash=StandardHash<K>::instance(),
                float loadFactor=DEFAULT_HASH_MAP_LOAD_FACTOR);
        ~HashMap();

        /**
         * Attributes
         **/
        float loadFactor() const { return _loadFactor; }
        void loadFactor(float newLoadFactor)
        {
            _loadFactor = newLoadFactor;
            _expandSize = static_cast<std::size_t>(ceilf(_loadFactor * _capacity));
        }

        /**
         * Observe
         **/
        bool contains(K const& key) const   { return (get(key) != 0); }
        bool empty() const                  { return _size == 0; }
        bool find(K const& key, V const*& value, K const** realKey=0) const
        {
            const Entry* entry(get(key));
            if( entry ) {
                value = entry->valuePtr();
                if( realKey ) {
                    *realKey = entry->keyPtr();
                }
                return true;
            } else {
                return false;
            }
        }
        void iterator(ConstIterator& iter) const
        {
            iter.initialize(_buckets, _capacity);
        }
        std::size_t size() const            { return _size; }
        const V& operator[](K const& key) const
        {
            const Entry* entry(get(key));
            if( entry ) {
                return entry->value();
            } else {
                throw Exception("Unknown key!");
            }
        }

        /**
         * Modify
         **/
        bool find(K const& key, V*& value, K const** realKey=0)
        {
            const hash_t hashcode(_hash.hash(key));
            std::size_t index(hashcode & _mask);
            Entry* entry(_buckets[index]);

            // See if already exists
            while( entry ) {
                if( entry->hashcode() == hashcode and entry->key() == key ) {
                    value = entry->valuePtr();
                    if( realKey ) {
                        *realKey = entry->keyPtr();
                    }
                    return true;
                } else {
                    entry = entry->next();
                }
            }

            // Not found
            return false;
        }

        V& operator[](K const& key)
        {
            const hash_t hashcode(_hash.hash(key));
            std::size_t index(hashcode & _mask);
            Entry* entry(_buckets[index]);

            // See if already exists
            while( entry ) {
                if( entry->hashcode() == hashcode and entry->key() == key ) {
                    return entry->value();
                } else {
                    entry = entry->next();
                }
            }

            // Expand if needed
            if( _size >= _expandSize ) {
                expand();
                index = hashcode & _mask;
            }

            // Create new entry
            entry = new(_allocator.alloc(sizeof(Entry), false)) Entry(key, hashcode, _buckets[index]);
            _initializer.initialize(entry->value());
            _buckets[index] = entry;
            ++_size;
            return entry->value();
        }
        
    private:
        void expand()
        {
            const std::size_t newCapacity(_capacity << 1);
            const std::size_t newMask(newCapacity - 1);
            Entry** newBuckets(static_cast<Entry**>(calloc(newCapacity, sizeof(Entry*))));
            for(std::size_t ii = 0; ii != _capacity; ++ii) {
                Entry* entry(_buckets[ii]);
                while(entry) {
                    Entry* const temp(entry->next());
                    const std::size_t newIndex(entry->hashcode() & newMask);
                    entry->next(newBuckets[newIndex]);
                    newBuckets[newIndex] = entry;
                    entry = temp;
                }
            }
            free(_buckets);
            
            _buckets = newBuckets;
            _capacity = newCapacity;
            _mask = newMask;

            // Reset _expandSize
            loadFactor(_loadFactor);
        }

        const Entry* get(K const& key) const
        {
            const hash_t hashcode(_hash.hash(key));
            Entry* entry(_buckets[hashcode & _mask]);
            while( entry ) {
                if( entry->hashcode() == hashcode and entry->key() == key ) {
                    return entry;
                } else {
                    entry = entry->next();
                }
            }
            return 0;
        }

        Allocator& _allocator;
        Initializer<V>& _initializer;
        Hash<K> const& _hash;
        float _loadFactor;
        std::size_t _size;
        std::size_t _expandSize;
        std::size_t _capacity;
        std::size_t _mask;
        Entry** _buckets;
    };
}

template <typename K, typename V>
promote::HashMap<K,V>::HashMap(std::size_t initCapacity,
                               Allocator& allocator,
                               Initializer<V>& initializer,
                               const Hash<K>& hash,
                               float initLoadFactor)
:   _allocator(allocator),
    _initializer(initializer),
    _hash(hash),
    _loadFactor(0),
    _size(0),
    _capacity(static_cast<std::size_t>(promote::leastPower2Above64(static_cast<uint64_t>(initCapacity), true))),
    _mask(_capacity - 1),
    _buckets(static_cast<Entry**>(calloc(_capacity, sizeof(Entry*))))
{
    loadFactor(initLoadFactor);
}

template <typename K, typename V>
promote::HashMap<K,V>::~HashMap()
{
    for(std::size_t ii = 0; ii != _capacity; ++ii) {
        Entry* entry(_buckets[ii]);
        while( entry ) {
            Entry* next(entry->next());
            entry->~Entry();
            _allocator.release(entry);
            entry = next;
        }
    }
    free(_buckets);
}

#endif /* PROMOTE_HASH_MAP_HPP_ */

