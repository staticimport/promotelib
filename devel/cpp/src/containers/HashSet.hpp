#ifndef PROMOTE_HASH_SET_HPP_
#define PROMOTE_HASH_SET_HPP_

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Allocator.hpp"
#include "Hash.hpp"
#include "Power.hpp"
#include "StandardAllocator.hpp"
#include "StandardHash.hpp"

namespace promote 
{
    const float DEFAULT_HASH_SET_LOAD_FACTOR = 0.6;

    template <typename T>
    class HashSet
    {
    public:
        class Entry
        {
        public:
            Entry(T const& item, Entry* next, hash_t hashValue) :
                _item(item), _next(next), _hashValue(hashValue)
            { }
            ~Entry() { }

            T const& item() const { return _item; }
        private:
            friend class HashSet<T>;

            const T _item;
            Entry* _next;
            const hash_t _hashValue;
        };


        HashSet(std::size_t initCapacity=16,
                Allocator& allocator=StandardAllocator::instance(),
                const Hash<T>& hash=StandardHash<T>::instance(),
                float initLoadFactor=promote::DEFAULT_HASH_SET_LOAD_FACTOR) :
            _allocator(allocator),
            _hash(hash),
            _loadFactor(0),
            _size(0),
            _capacity(static_cast<std::size_t>(promote::leastPower2Above64(static_cast<uint64_t>(initCapacity), true))),
            _mask(_capacity - 1),
            _buckets(static_cast<Entry**>(calloc(_capacity, sizeof(Entry*))))
        {
            loadFactor(initLoadFactor);
        }
        
        ~HashSet()
        {
            for(std::size_t ii = 0; ii != _capacity; ++ii) {
                Entry* entry(_buckets[ii]);
                while( entry ) {
                    Entry* next(entry->_next);
                    release(entry);
                    entry = next;
                }
            }
            free(_buckets);
        }

        // Attributes
        float loadFactor() const { return _loadFactor; }
        void loadFactor(float newLoadFactor)
        {
            _loadFactor = newLoadFactor;
            _expandSize = static_cast<std::size_t>(ceilf(_loadFactor * _capacity));
        }

        // Observe
        bool contains(T const& item)
        {
            const Entry* entry = _buckets[_hash.hash(item) & _mask];
            while( entry ) {
                if( entry->_item == item )
                    return true;
                else
                    entry = entry->_next;
            }
            return false;
        }
        bool empty() const { return _size == 0; }
        std::size_t size() const { return _size; }

        // Modify
        bool add(T const& item)
        {
            // Expand if necessary
            if( _size >= _expandSize ) {
                expand();
            }

            // Calculate position from hash
            const hash_t hashValue(_hash.hash(item));
            const std::size_t index(hashValue & _mask);

            // Check if already contained
            Entry* entry(_buckets[index]);
            while( entry ) {
                if( hashValue == entry->_hashValue and item == entry->_item )
                    return false;
                else
                    entry = entry->_next;
            }

            // Add new entry
            void* space(_allocator.alloc(sizeof(Entry), false));
            if( space == 0 ) {
                // TODO: throw exception
            }
            entry = new(space) Entry(item, _buckets[index], hashValue);
            if( entry ) {
                _buckets[index] = entry;
                ++_size;
                return true;
            } else
                return false;
        }

        bool remove(T const& item)
        {
            const hash_t hashValue = hash(item);
            const std::size_t index = hashValue & _mask;

            Entry* entry(_buckets[index]), previous(0);
            while( entry ) {
                if( item == entry->_item ) {
                    (previous ? previous->_next : _buckets[index]) = entry->_next;
                    release(entry);
                    --_size;
                    return true;
                } else
                    entry = (previous = entry)->_next;
            }

            return false;
        }
    private:
        void expand()
        {
            const std::size_t newCapacity = _capacity << 1;
            const std::size_t newMask = newCapacity - 1;
            std::size_t newIndex;

            Entry** newBuckets = static_cast<Entry**>(calloc(newCapacity, sizeof(Entry*)));
            Entry* entry, *temp;
            for(std::size_t ii = 0; ii != _capacity; ++ii) {
                entry = _buckets[ii];
                while(entry) {
                    temp = entry->_next;
                    newIndex = entry->_hashValue & newMask;
                    entry->_next = newBuckets[newIndex];
                    newBuckets[newIndex] = entry;
                    entry = temp;
                }
            }
            free(_buckets);
            _buckets = newBuckets;
            _capacity = newCapacity;
            _mask = newMask;
            loadFactor(_loadFactor);
        }

        void release(Entry* entry)
        {
            entry->~Entry();
            _allocator.release(entry);
        }


        Allocator& _allocator;
        const Hash<T>& _hash;
        float _loadFactor;
        std::size_t _size;
        std::size_t _capacity;
        std::size_t _mask;
        std::size_t _expandSize; // = ceil(_loadFactor * _capacity)
        Entry** _buckets;
    };

}

#endif /* PROMOTE_HASH_SET_HPP_ */
