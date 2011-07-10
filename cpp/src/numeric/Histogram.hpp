#ifndef PROMOTE_HISTOGRAM_HPP_
#define PROMOTE_HISTOGRAM_HPP_

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "PoolQueueAllocator.hpp"
#include "TreapMap.hpp"

namespace promote
{
    template <typename K>
    class Histogram
    {
    public:
        Histogram();
        ~Histogram() { }

        // Const
        bool isEmpty() const                            { return _sampleCount == 0; }
        typename Passable<K const>::Type max() const    { return _max; }
        typename Passable<K const>::Type min() const    { return _min; }
        std::size_t sampleCount() const                 { return _sampleCount; }

        template <typename T>
        T mean() const
        {
            T value(static_cast<T>(_max));
            typename TreapMap<K,uint64_t>::ConstIterator iter;
            _map.iterator(iter);
            while( iter.hasNext() ) {
                TreapMapEntry<K,uint64_t> const& entry(iter.next());
                K const& key(entry.key());
                uint64_t const count(entry.value());
                value = value + (static_cast<T>(key) * (iter.hasNext() ? count : (count-1)));
            }
            return value / _sampleCount;
        }

        template <typename T>
        T median() const
        {
            typename TreapMap<K,uint64_t>::ConstIterator iter;
            _map.iterator(iter);
            for(std::size_t ii = (_map.size()-1) >> 1; ii; --ii) {
                iter.next();
            }
            if( (_map.size() & 1) ) {
                return static_cast<T>(iter.next().key());
            } else {
                return static_cast<T>(iter.next().key() + iter.next().key()) / 2;
            }
        }

        template <typename T>
        T standardDeviation() const
        {
            double const mean(Histogram<K>::mean<double>());
            double value(0);
            typename TreapMap<K,uint64_t>::ConstIterator iter;
            _map.iterator(iter);
            while( iter.hasNext() ) {
                TreapMapEntry<K,uint64_t> const& entry(iter.next());
                double const diffFromMean(static_cast<double>(entry.key()) - mean);
                value += entry.value() * (diffFromMean * diffFromMean);
            }
            return static_cast<T>(sqrt(value / _sampleCount));
        }

        // Non-Const
        void add(typename Passable<K const>::Type key)
        {
            uint64_t* valuePtr(0);
            if( _map.find(key, valuePtr) ) {
                ++(*valuePtr);
            } else {
                _map[key] = 1;
                if( _sampleCount ) {
                    if( key < _min ) {
                        _min = key;
                    } else if( _max < key ) {
                        _max = key;
                    }
                } else {
                    _min = _max = key;
                }
            }
            ++_sampleCount;
        }
        void clear() 
        { 
            _map.clear();
            _sampleCount = 0;
        }
    private:
        PoolQueueAllocator<false,false> _pool;
        TreapMap<K,uint64_t> _map;
        std::size_t _sampleCount;
        K _min;
        K _max;
    };
}

/****
 * Implementation
 ****/
template <typename K>
promote::Histogram<K>::Histogram()
:   _pool(sizeof(TreapMapEntry<K,uint64_t>), 32),
    _map(_pool),
    _sampleCount(0)
{ }

#endif /* PROMOTE_HISTOGRAM_HPP_ */

