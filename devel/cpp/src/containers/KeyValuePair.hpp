#ifndef PROMOTE_KEY_VALUE_PAIR_HPP_
#define PROMOTE_KEY_VALUE_PAIR_HPP_

#include "Utilities.hpp"

namespace promote
{
    template <typename K, typename V>
    class KeyValuePair
    {
    public:
        KeyValuePair() { }
        KeyValuePair(typename Passable<K const>::Type key)
        :   _key(key)
        { }
        KeyValuePair(typename Passable<K const>::Type key,
                     typename Passable<V const>::Type value)
        :   _key(key), _value(value)
        { }
        virtual ~KeyValuePair() { }

        /* Const */
        typename Passable<K const>::Type key() const    { return _key; }
        typename Passable<V const>::Type value() const  { return _value; }

        /* Non-Const */
        K& key()    { return _key; }
        V& value()  { return _value; }
        KeyValuePair& operator=(KeyValuePair const& pair)
        {
            if( this != &pair ) {
                _key = pair.key();
                _value = pair.value();
            }
            return *this;
        }
    private:
        K _key;
        V _value;
    };

    template <typename K, typename V>
    class ComparableKeyValuePair : public KeyValuePair<K,V>
    {
    public:
        ComparableKeyValuePair() : KeyValuePair<K,V>() { }
        ComparableKeyValuePair(typename Passable<K const>::Type key)
        :   KeyValuePair<K,V>(key)
        { }
        ComparableKeyValuePair(typename Passable<K const>::Type key,
                               typename Passable<V const>::Type value)
        :   KeyValuePair<K,V>(key,value)
        { }

        bool operator<(KeyValuePair<K,V> const& pair) const    
        {
            return KeyValuePair<K,V>::key() < pair.key(); 
        }
        bool operator<=(KeyValuePair<K,V> const& pair) const
        {
            return KeyValuePair<K,V>::key() <= pair.key(); 
        }
        bool operator>(KeyValuePair<K,V> const& pair) const
        {
            return KeyValuePair<K,V>::key() > pair.key(); 
        }
        bool operator>=(KeyValuePair<K,V> const& pair) const
        {
            return KeyValuePair<K,V>::key() >= pair.key(); 
        }
        bool operator==(KeyValuePair<K,V> const& pair) const
        {
            return KeyValuePair<K,V>::key() == pair.key(); 
        }
        bool operator!=(KeyValuePair<K,V> const& pair) const
        {
            return KeyValuePair<K,V>::key() != pair.key(); 
        }
    };
}

#endif /* PROMOTE_KEY_VALUE_PAIR_HPP_ */

