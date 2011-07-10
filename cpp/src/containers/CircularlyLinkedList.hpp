#ifndef PROMOTE_CIRCULARLY_LINKED_LIST_HPP_
#define PROMOTE_CIRCULARLY_LINKED_LIST_HPP_

#include <cassert>

#include "Allocator.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "StandardAllocator.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename T>
    class CircularlyLinkedList
    {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        class Node
        {
        public:
            //Node(typename Passable<T const>::Type item)
            Node(T const& item)
            :   _item(item), _previous(0), _next(0)
            { }

            /* Const */
            //typename Passable<T const>::Type item() const { return _item; }
            T const& item() const { return _item; }
            Node* next() const     { return _next; }
            Node* previous() const { return _previous; }

            /* Non-Const */
            //typename Passable<T>::Type item()   { return _item; }
            T& item() { return _item; }
            void next(Node* const next)         { _next = next; }
            void previous(Node* const previous) { _previous = previous; }
        private:
            T _item;
            Node *_previous, *_next;
        };
    private:   
        template <typename NODE, typename V>
        class CLLIterator : public promote::Iterator<V>
        {
        public:
            CLLIterator(NODE* node) : _nextNode(node) { }

            bool hasNext() const { return _nextNode != 0; }
            typename Passable<V>::Type next()
            {
                typename Passable<V>::Type value(_nextNode->item());
                _nextNode = _nextNode->next();
                return value;
            }
        private:
            NODE* _nextNode;
        };
    
    public:
        static std::size_t const ALLOC_SIZE = sizeof(Node);

        typedef CLLIterator<Node const,T const> ConstIterator;
        typedef CLLIterator<Node,T> Iterator;

        CircularlyLinkedList(Allocator& allocator=StandardAllocator::instance())
        :   _allocator(allocator), _front(0), _size(0)
        { }
        ~CircularlyLinkedList()
        {
            while( !empty() ) {
                removeBack();
            }
        }

        /* Const */
        bool empty() const                              { return _size == 0; }
        typename Passable<T const>::Type back() const   { return _front->previous()->item(); }
        Node const* backNode() const                    { return _front->previous(); }
        typename Passable<T const>::Type front() const  { return _front->item(); }
        Node const* frontNode() const                   { return _front; }
        std::size_t size() const                        { return _size; }
        typename Passable<T>::Type operator[](std::size_t const index) const
        {
            Node* node(_front);
            for(std::size_t ii = 0; ii != index; ++ii) {
                node = node->next();
            }
            return node->item();
        }

        /* Non-Const */
        typename Passable<T>::Type back()   { return _front->previous()->item(); }
        Node* backNode()                    { return _front->previous(); }
        typename Passable<T>::Type front()  { return _front->item(); }
        Node* frontNode()                   { return _front; }
        void pushBack(typename Passable<T const>::Type item)
        {
            Node* newBack(createNode(item));
            if( newBack ) {
                if( _front ) {
                    newBack->next(_front);
                    newBack->previous(_front->previous());
                    newBack->next()->previous(newBack);
                    newBack->previous()->next(newBack);
                } else {
                    newBack->next(newBack);
                    newBack->previous(newBack);
                    _front = newBack;
                }
                ++_size;
            } else {
                throw new Exception("Failed to create node.");
            }
        }
        void pushFront(typename Passable<T const>::Type item)
        {
            Node* newFront(createNode(item));
            if( newFront ) {
                if( _front ) {
                    newFront->next(_front);
                    newFront->previous(_front->previous());
                    _front->previous()->next(newFront);
                    _front->previous(newFront);
                } else {
                    newFront->next(newFront);
                    newFront->previous(newFront);
                }
                _front = newFront;
                ++_size;
            } else {
                throw new Exception("Failed to create node.");
            }
        }
        void removeBack()
        {
            Node* const theBack(_front->previous());
            if( --_size ) {
                _front->previous(theBack->previous());
                _front->previous()->next(_front);
            } else {
                _front = 0;
            }
            _allocator.release(theBack);
        }
        typename Passable<T const>::Type operator[](std::size_t const index)
        {
            Node* node(_front);
            for(std::size_t ii = 0; ii != index; ++ii) {
                node = node->next();
            }
            return node->item();
        }
    private:
        Node* createNode(typename Passable<T const>::Type item)
        {
            void *const space(_allocator.alloc(sizeof(Node),false));
            return space ? new(space) Node(item) : 0;
        }

        /* Fields */
        Allocator& _allocator;
        Node* _front;
        std::size_t _size;
    };
}

#endif /* PROMOTE_CIRCULARLY_LINKED_LIST_HPP_ */

