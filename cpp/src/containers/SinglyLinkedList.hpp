#ifndef PROMOTE_SINGLY_LINKED_LIST_HPP_
#define PROMOTE_SINGLY_LINKED_LIST_HPP_

#include <cstdio>
#include <new>

#include "Allocator.hpp"
#include "Iterator.hpp"
#include "StandardAllocator.hpp"

namespace promote
{
    template <typename T>
    class SinglyLinkedList
    {
    public:
        class Node
        {
        public:
            Node(T const& item_, Node* next_)
            :   _item(item_), _next(next_)
            { }

            /* Const */
            T const& item() const       { return _item; }
            Node const* next() const    { return _next; }

            /* Non-Const */
            T& item()                   { return _item; }
            Node* next()                { return _next; }
        private:
            void append(Node* const newNext)
            {
                newNext->_next = _next;
                _next = newNext;
            }

            friend class SinglyLinkedList<T>;

            T _item;
            Node* _next;
        };
    private:
        /**
         * Iterators
         **/
        template <typename SLL, typename NODE, typename V>
        class SLLIterator : public promote::Iterator<V>
        {
        public:
            SLLIterator(SLL& list) : _node(list._head) { }

            bool hasNext() { return (_node != 0); }
            typename Passable<V>::Type next()
            {
                V& value(_node->item());
                _node = _node->next();
                return value;
            }
        private:
            NODE* _node;
        };

    public:
        static const std::size_t ALLOC_SIZE = sizeof(Node);

        typedef SLLIterator<SinglyLinkedList<T> const,Node const,T const> ConstIterator;
        typedef SLLIterator<SinglyLinkedList<T>,Node,T> Iterator;

        friend class SLLIterator<SinglyLinkedList<T> const,Node const,T const>;
        friend class SLLIterator<SinglyLinkedList<T>,Node,T>;

        SinglyLinkedList(Allocator& allocator=StandardAllocator::instance()) :
            _allocator(allocator), _head(0), _tail(0), _size(0)
        { }
        ~SinglyLinkedList();

        // Observe
        T const& get(std::size_t index) const
        {
            Node *node(_head);
            for( ; index; --index)
                node = node->next();
            return node->item();
        }
        T const& getBack()   const          { return _tail->item(); }
        Node const* getBackNode() const     { return _tail; }
        T const& getFront()  const          { return _head->item(); }
        Node const* getFrontNode() const    { return _head; }
        bool isEmpty() const                { return (_size == 0); }
        std::size_t size() const            { return _size; }
        T const& operator[](std::size_t index) const {
            return get(index);
        }

        // Modify
        void clear()
        {
            while( _size )
                removeFront();
        }

        bool insert(T& item, std::size_t index)
        {
            bool result(false);
            if( index and index != _size ) {
                Node* const newNode = alloc(item, 0);
                if( newNode ) {
                    Node *node = _head;
                    for(--index; index; --index) {
                        node = node->next();
                    }
                    node->append(newNode);
                    ++_size;
                    result = true;
                }
            } else if( index ) {
                result = pushBack(item);
            } else {
                result = pushFront(item);
            }
            return result;
        }

        bool pushBack(T const& item)
        {
            Node *const newNode = alloc(item, 0);
            if( newNode ) {
                if( _tail ) {
                    _tail = (_tail->_next = newNode);
                } else {
                    _tail =_head = newNode;
                }
                ++_size;
                return true;
            } else
                return false;
        }

        bool pushFront(T& item)
        {
            Node* const newNode = alloc(item, _head);
            if( newNode ) {
                if( _head ) {
                    _head = newNode;
                } else {
                    _head = _tail = newNode;
                }
                ++_size;
                return true;
            } else
                return false;
        }

        bool remove(std::size_t index, T* copy = 0)
        {
            if( index ) {
                Node* node(_head);
                Node* toRemove(0);
                for(--index; index; --index)
                    node = node->next();
                toRemove = node->next();
                if( copy )
                    *copy = toRemove->item();
                node->_next = toRemove->next();
                release(toRemove);
                --_size;
                return true;
            } else
                return removeFront(copy);
        }

        bool removeFront(T* copy = 0)
        {
            Node* oldHead = _head;
            if( copy )
                *copy = oldHead->item();
            if( --_size > 1 )
                _head = oldHead->next();
            else
                _head = _tail = oldHead->next();
            release(oldHead);
            return true;
        }
    private:
        Node* alloc(T const& item, Node* next)
        {
            void* space = _allocator.alloc(ALLOC_SIZE, false);
            return space ? new(space) Node(item, next) : 0;
        }

        void release(Node *node)
        {
            node->~Node();
            _allocator.release(node);
        }

        Allocator& _allocator;
        Node *_head, *_tail;
        std::size_t _size;
    };
}

template <typename T>
promote::SinglyLinkedList<T>::~SinglyLinkedList()
{
    Node* node(_head);
    Node* temp(0);
    while( node ) {
        temp = node->next();
        release(node);
        node = temp;
    }
}

#endif /* PROMOTE_SINGLY_LINKED_LIST_HPP_ */
