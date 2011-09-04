#ifndef PROMOTE_DOUBLY_LINKED_LIST_HPP_
#define PROMOTE_DOUBLY_LINKED_LIST_HPP_

#include <new>

#include "Allocator.hpp"
#include "Exception.hpp"
#include "Iterator.hpp"
#include "StandardAllocator.hpp"

namespace promote
{
    template <typename T>
    class DoublyLinkedList
    {
    private:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        struct Node
        {
        public:
            Node(T const& item)
            :   item(item), previous(0), next(0)
            { }

            ~Node() { }

            T item;
            Node* previous;
            Node* next;
        };

    public:
        static const std::size_t ALLOC_SIZE = sizeof(Node);

        /**
         * Iterators
         **/
        class ConstIterator : public promote::Iterator<T>
        {
        public:
            ConstIterator() : _nextNode(0) { }
            ConstIterator(const DoublyLinkedList<T>& list)
            {
                list.iterator(*this);
            }
            ~ConstIterator() { }

            bool hasNext() { return _nextNode != 0; }
            typename Passable<T const>::Type next()
            {
                typename Passable<T const>::Type item(_nextNode->item);
                _nextNode = _nextNode->next;
                return item;
            }
        private:
            void initialize(const Node* head)
            {
                _nextNode = head;
            }

            friend class DoublyLinkedList<T>;

            const Node* _nextNode;
        };

        class Iterator : public promote::Iterator<T>
        {
        public:
            Iterator() : _nextNode(0) { }
            Iterator(DoublyLinkedList<T>& list)
            {
                initialize(list._head);
            }
            ~Iterator() { }

            bool hasNext()  { return _nextNode != 0; }
            typename Passable<T>::Type next()
            {
                typename Passable<T>::Type item(_nextNode->item);
                _nextNode = _nextNode->next;
                return item;
            }
        private:
            void initialize(Node* head)
            {
                _nextNode = head;
            }

            friend class DoublyLinkedList<T>;

            Node* _nextNode;
        };

        /**
         * Constructor/Destructor
         **/
        DoublyLinkedList(Allocator& allocator=StandardAllocator::instance()) :
            _allocator(allocator), _head(0), _tail(0), _size(0)
        { }
        ~DoublyLinkedList()
        {
            clear();
        }

        // Observe
        T const& back() const       { return _tail->item; }
        T const& front() const      { return _head->item; }
        bool empty() const          { return (_size == 0); }
        void iterator(ConstIterator& iter) const
        {
            iter.initialize(_head);
        }
        std::size_t size() const    { return _size; }
        T const& operator[](std::size_t index) const
        {
            if( (index << 1) < _size ) {
                // Forward iteration
                const Node* node(_head);
                for(; index; --index) {
                    node = node->next;
                }
                return node->item;
            } else {
                // Iterate from tail
                const Node* node(_tail);
                for(std::size_t ii = _size - index - 1; ii; --ii) {
                    node = node->previous;
                }
                return node->item;
            }
        }

        // Modify
        void clear()
        {
            Node* head(_head);
            while( head ) {
                Node* const newHead(head->next);
                _allocator.release(head);
                head = newHead;
            }
            _head = _tail = 0;
            _size = 0;
        }

        void insert(T const& item, std::size_t index)
        {
            if( index and index != _size ) {
                // Create
                void* space(_allocator.alloc(ALLOC_SIZE,false));
                if( space == 0 ) {
                    throw Exception("Failed to alloc entry for DLL insert.");
                }
                Node* const newNode(new(space) Node(item));

                // Find location for new node
                Node* previous(0);
                Node* next(0);
                if( (index << 1) < _size ) {
                    previous = _head;
                    for(--index; index; --index) {
                        previous = previous->next;
                    }
                    next = previous->next;
                } else {
                    next = _tail;
                    for(std::size_t ii = _size - index - 2; ii; --ii) {
                        next = next->previous;
                    }
                    previous = next->previous;
                }

                // Insert
                newNode->next = next;
                next->previous = newNode;
                newNode->previous = previous;
                previous->next = newNode;
                ++_size;
            } else if( index ) {
                pushBack(item);
            } else {
                pushFront(item);
            }
        }

        void iterator(Iterator& iterator)
        {
            iterator.initialize(_head);
        }

        void pushBack(T const& item)
        {
            // Create
            void *space(_allocator.alloc(ALLOC_SIZE,false));
            if( space == 0 ) {
                throw Exception("Failed to alloc entry for DLL pushBack.");
            }
            Node* const newTail(new(space) Node(item));

            // Append
            if( _tail ) {
                _tail->next = newTail;
                newTail->previous = _tail;
            } else {
                _head = newTail;
            }
            _tail = newTail;
            ++_size;
        }

        void pushFront(T const& item)
        {
            // Create
            void* space(_allocator.alloc(ALLOC_SIZE,false));
            if( space == 0 ) {
                throw Exception("Failed to alloc entry for DLL pushFront.");
            }
            Node* const newHead(new(space) Node(item));

            // Prepend
            if( _head ) {
                _head->previous = newHead;
                newHead->next = _head;
            } else {
                _tail = newHead;
            }
            _head = newHead;
            ++_size;
        }

        void remove(std::size_t index, T* copy = 0)
        {
            // Find
            Node* node;
            if( (index << 1) < _size ) {
                node = _head;
                for( ; index; --index) {
                    node = node->next;
                }
            } else {
                node = _tail;
                for(std::size_t ii = _size - index - 1; ii; --ii) {
                    node = node->previous;
                }
            }

            // Copy value if desired
            if( copy ) {
                *copy = node->item;
            }

            // Remove
            if( node->previous ) {
                node->previous->next = node->next;
            } else {
                _head = node->next;
            }
            if( node->next ) {
                node->next->previous = node->previous;
            } else {
                _tail = node->previous;
            }
            _allocator.release(node);
            --_size;
        }

        void removeBack(T* copy = 0)
        {
            // Copy value if desired
            if( copy ) {
                *copy = _tail->item;
            }

            // Remove
            Node* const newTail(_tail->_previous);
            if( newTail ) {
                newTail->next = 0;
            } else {
                _head = 0;
            }
            _allocator.release(_tail);
            _tail = newTail;
            --_size;
        }

        void removeFront(T* copy = 0)
        {
            // Copy value if desired
            if( copy ) {
                *copy = _head->item;
            }
            

            // Remove
            Node* const newHead(_head->next);
            if( newHead ) {
                newHead->previous = 0;
            } else {
                _tail = 0;
            }
            _allocator.release(_head);
            _head = newHead;
            --_size;
        }
    private:
        Allocator& _allocator;
        Node *_head, *_tail;
        std::size_t _size;
    };
}

#endif /* PROMOTE_DOUBLY_LINKED_LIST_HPP_ */
