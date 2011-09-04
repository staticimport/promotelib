#ifndef PROMOTE_MAP_HPP_
#define PROMOTE_MAP_HPP_

namespace promote
{
    template <typename K, typename V>
    class MapEntry
    {
    public:
        MapEntry(K const& key);
        virtual ~MapEntry() { }

        /* Const */
        K const& key() const    { return _key; }
        V const& value() const  { return _value; }

        /* Non-Const */
        //K& key()    { return _key; }
        V& value()  { return _value; }
    protected:
        const K _key;
        V _value;
    };

    template <typename K, typename V>
    class Map
    {
    public:
        virtual ~Map() { }
    };
}

/***
 * Implementation
 ***/
template <typename K, typename V>
promote::MapEntry<K,V>::MapEntry(K const& key)
:   _key(key)
{
}

#endif /* PROMOTE_MAP_HPP_ */

