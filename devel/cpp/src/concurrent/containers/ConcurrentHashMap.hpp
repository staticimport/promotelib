#ifndef PROMOTE_CONCURRENT_HASH_MAP_HPP_
#define PROMOTE_CONCURRENT_HASH_MAP_HPP_

#include <cstdlib>

#include "Allocator.hpp"
#include "Hash.hpp"
#include "StandardAllocator.hpp"
#include "StandardHash.hpp"

namespace promote
{
    template <typename K, typename V>
    class ConcurrentHashMap {
    public:
        class Entry {
        public:
            K const& key() const    { return _key; }
            V const& value() const  { return _value; }
        private:
            Entry(K const& key, hash_t hashcode, Entry* next = 0)
            :   _key(key), _hashcode(hashcode), _next(next)
            { }

            hash_t hashcode() const { return _hashcode; }
            Entry* next() const     { return _next; }
            Entry* previous() const { return _previous; }

            Entry*& next()          { return _next; }
            Entry* volatile& previous() { return _previous; }

            friend class HashMap<K,V>;

            K const _key;
            V _value;
            Entry* _next;
            Entry* volatile _previous;
        };

        static std::size_t const ALLOC_SIZE = sizeof(Entry);

        ConcurrentHashMap(std::size_t const capacity,
                          Allocator& allocator=promote::StandardAllocator::instance(),
                          Hash<K> const& hash=StandardHash<K>::instance(),
                          float loadFactor=DEFAULT_LOAD_FACTOR);
        ~ConcurrentHashMap();

        /**
         * Attributes
         **/
        float loadFactor() const { return _loadFactor; }

        /**
         * Observe
         **/

        /**
         * Modify
         **/
        V& operator[](K const& key) {
            hash_t const hashcode(_hash.hash(key));
            std::size_t const index(hashcode & _mask);
            
            // See if already exists
            Entry* entry(get(key,hashcode));
            if( entry ) {
                return entry->value();
            }

            // Create new entry
            entry = new(_allocator.alloc(sizeof(Entry),false)) Entry(key, hashcode);
        }
    private:
        Entry const* get(K const& key) const {
            return get(key, _hash.hash(key));
        }
        Entry const* get(K const& key, hash_t const hashcode) const
        {
            hash_t const hashcode(_hash.hash(key));
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
        Hash<K>& const _hash;
        float const _loadFactor;
        std::size_t const _capacity;
        std::size_t const _mask;
        Entry* volatile* const _buckets;
    };
}

#endif /* PROMOTE_CONCURRENT_HASH_MAP_HPP_ */

