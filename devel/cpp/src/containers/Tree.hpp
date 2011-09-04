#ifndef PROMOTE_TREE_HPP_
#define PROMOTE_TREE_HPP_

namespace promote
{
    class TreeEntry
    {
    public:
        TreeEntry();
        virtual ~TreeEntry() { }
        
        /* Const */
        virtual TreeEntry const* left() const   { return _left; }
        virtual TreeEntry const* parent() const { return _parent; }
        virtual TreeEntry const* right() const  { return _right; }

        /* Non-Const */
        virtual TreeEntry* left()   { return _left; }
        virtual TreeEntry* parent() { return _parent; }
        virtual TreeEntry* right()  { return _right; }
        virtual TreeEntry* left(TreeEntry* const newLeft)
        {
            return (_left = newLeft);
        }
        virtual TreeEntry* parent(TreeEntry* const newParent)
        {
            return (_parent = newParent);
        }
        virtual TreeEntry* right(TreeEntry* const newRight)
        {
            return (_right = newRight);
        }
        void rotateLeft()
        {
            TreeEntry* const oldParent(_parent);
            _parent = _right;
            _right = _right->_left;
            if( _right ) {
                _right->_parent = this;
            }
            _parent->_left = this;
            _parent->_parent = oldParent;
            if( oldParent ) {
                if( oldParent->_left == this ) {
                    oldParent->_left = _parent;
                } else {
                    oldParent->_right = _parent;
                }
            }
        }

        void rotateRight()
        {
            TreeEntry* const oldParent(_parent);
            _parent = _left;
            _left = _left->_right;
            if( _left ) {
                _left->_parent = this;
            }
            _parent->_right = this;
            _parent->_parent = oldParent;
            if( oldParent ) {
                if( oldParent->_left == this ) {
                    oldParent->_left = _parent;
                } else {
                    oldParent->_right = _parent;
                }
            }
        }

        TreeEntry *_left, *_parent, *_right;
    };
}

#endif /* PROMOTE_TREE_HPP_ */

