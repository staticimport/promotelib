#ifndef PROMOTE_TREE_MAP_HPP_
#define PROMOTE_TREE_MAP_HPP_

#include "Map.hpp"
#include "Tree.hpp"

namespace promote
{
    template <typename K, typename V>
    class TreeMapEntry : public TreeEntry, public MapEntry<K,V>
    {
    public:
        TreeMapEntry(K const& key);
        virtual ~TreeMapEntry() { }
    };

    template <typename K, typename V>
    class TreeMap
    {
    public:

    protected:
        TreeMapEntry<K,V>* _root;
    };
};

/***
 * Implementation
 ***/
template <typename K, typename V>
promote::TreeMapEntry<K,V>::TreeMapEntry(K const& key)
:   MapEntry<K,V>(key)
{
}

#endif /* PROMOTE_TREE_MAP_HPP_ */

