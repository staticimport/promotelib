#ifndef PROMOTE_TREE_UTILS_H_
#define PROMOTE_TREE_UTILS_H_

#define PROMOTE_TREE_CONTAINS(root, type, k, comparator, result) \
        { \
            type* entry = root; \
            *(result) = false; \
            while( entry ) { \
                switch(pmt_comparator_compare(comparator, k, entry->key)) { \
                    case PMT_ORDER_INCREASING: \
                        entry = entry->left; \
                        break; \
                    case PMT_ORDER_DECREASING: \
                        entry = entry->right; \
                        break; \
                    case PMT_ORDER_SAME: \
                        *(result) = true; \
                        entry = 0; \
                        break; \
                } \
            } \
        }

#define PROMOTE_TREE_FIRST_KEY(root, type, result) \
        { \
            type* entry = root; \
            while( entry->left ) { \
                entry = entry->left; \
            } \
            *(result) = entry->key; \
        }

#define PROMOTE_TREE_FIRST_VALUE(root, type, result) \
        { \
            type* entry = root; \
            while( entry->left ) { \
                entry = entry->left; \
            } \
            *(result) = entry->value; \
        }

#define PROMOTE_TREE_LAST_KEY(root, type, result) \
        { \
            type* entry = root; \
            while( entry->right ) { \
                entry = entry->right; \
            } \
            *(result) = entry->key; \
        }

#define PROMOTE_TREE_LAST_VALUE(root, type, result) \
        { \
            type* entry = root; \
            while( entry->right ) { \
                entry = entry->right; \
            } \
            *(result) = entry->value; \
        }

#define PROMOTE_TREE_ROTATE_LEFT(nodeRaw, type) \
        { \
            type* node = nodeRaw; \
            type* oldParent = node->parent; \
            if( (node->right = (node->parent = node->right)->left) ) \
                node->right->parent = 0; \
            node->parent->left = node; \
            if( (node->parent->parent = oldParent) ) { \
                if( oldParent->left == node ) { \
                    oldParent->left = node->parent; \
                } else { \
                    oldParent->right = node->parent; \
                } \
            } \
        }

#define PROMOTE_TREE_ROTATE_RIGHT(nodeRaw, type) \
        { \
            type* node = nodeRaw; \
            type* oldParent = node->parent; \
            if( (node->left = (node->parent = node->left)->right) ) \
                node->left->parent = 0; \
            node->parent->right = node; \
            if( (node->parent->parent = oldParent) ) { \
                if( oldParent->left == node ) { \
                    oldParent->left = node->parent; \
                } else { \
                    oldParent->right = node->parent; \
                } \
            } \
        }

#endif /* PROMOTE_TREE_UTILS_H_ */

