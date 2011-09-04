#ifndef PROMOTE_VOLATILE_PTR_HPP_
#define PROMOTE_VOLATILE_PTR_HPP_

#include <pthread.h>

#include "Atomic.hpp"
#include "Mutex.hpp"
#include "Observed.hpp"
#include "Threads.hpp"

namespace promote
{
    template <typename T, bool MULTI_WRITERS=true>
    class VolatileRawPtr {
    public:
        VolatileRawPtr() : _ptr(NULL) { }
        
        // Const
        T* getPtr() const { return _ptr.fetch(); }

        // Non-Const
        bool compareAndSwapPtr(T* const comparePtr, T* const swapPtr) {
            return _ptr.compareAndSwap(comparePtr, swapPtr);
        }
        void setPtr(T* const newPtr) { _ptr.set(newPtr); }
    private:
        Atomic<T*> _ptr;
    };

    template <typename T>
    class VolatileRawPtr<T,false> {
    public:
       VolatileRawPtr() : _ptr(NULL) { }

       // Const
       T* getPtr() const { return _ptr; }

       // Non-Const
       bool compareAndSwapPtr(T* const comparePtr, T* const swapPtr) {
           if( comparePtr == _ptr ) {
               _ptr = swapPtr;
               return true;
           } else {
               return false;
           }
       }
       void setPtr(T* const newPtr) { _ptr = newPtr; }
    private:
        T* volatile _ptr;
    };
    
    template <typename T, bool MULTI_WRITERS> class VolatilePtrThreadSnapshotFactory;

    template <typename T, bool MULTI_WRITERS=true>
    class VolatilePtr {
    public:
        /*class Snapshot {
        public:
            Snapshot(T* const ptr, unsigned& refCounter)
            :   _ptr(ptr), _refCounter(refCounter)
            {
                ++_refCounter;
            }
            Snapshot(Snapshot const& snapshot)
            :   _ptr(snapshot.ptr()), _refCounter(snapshot._refCounter)
            {
                ++_refCounter;
            }
            ~Snapshot() { --_refCounter; }

            T* ptr() const { return _ptr; } // TODO
        private:
            T const* _ptr;
            unsigned& _refCounter;
        };*/

        VolatilePtr() : _headNode(0) { }
        ~VolatilePtr();

        // Const
        T* getRawPtr() const                { return _rawPtr.getPtr(); }
        bool isRawPtrReferenced(T* const ptr) const;

        // Non-Const
        bool compareAndSwapRawPtr(T* const comparePtr, T* const swapPtr) {
            return _rawPtr.compareAndSwapPtr(comparePtr, swapPtr);
        }
        void setRawPtr(T* const newRawPtr)  { _rawPtr.setPtr(newRawPtr); }
        Observed<T*> snapshot();
    private:
        struct ThreadNode {
            ThreadNode(pthread_t const pthread, 
                       VolatileRawPtr<T,MULTI_WRITERS>& rawPtr,
                       ThreadNode* nextNode);
            ~ThreadNode();

            pthread_t const pthread;
            ThreadNode* const nextNode;
            VolatilePtrThreadSnapshotFactory<T,MULTI_WRITERS>* snapshotFactory;
        };

        ThreadNode* findNodeForThread(pthread_t const pthread,
                                      ThreadNode* const beginNode,
                                      ThreadNode* const endNode)
        {
            ThreadNode* node(beginNode);
            while( node != endNode and !Threads::equal(pthread, node->pthread) ) {
                node = node->nextNode;
            }
            return node;
        }

        VolatileRawPtr<T,MULTI_WRITERS> _rawPtr;
        ThreadNode* volatile _headNode;
        Mutex _newNodeMutex;
    };
    
    template <typename T, bool MULTI_WRITERS=true>
    class VolatilePtrThreadSnapshotFactory {
    private:
        class RefCounter : public Observer<T*> {
        public:
            RefCounter() : _refCounter(0) { }
            ~RefCounter() { }

            void decrement()                            { --_refCounter; }
            void increment()                            { ++_refCounter; }
            void onConstructor(Observed<T*>& observed)  { increment(); }
            void onDestructor(Observed<T*>& observed)   { decrement(); }
            unsigned value() const                      { return _refCounter; }
        private:
            unsigned _refCounter;
        };
    public:
        VolatilePtrThreadSnapshotFactory(VolatileRawPtr<T,MULTI_WRITERS>& volatileRawPtr)
        :   _volatileRawPtr(volatileRawPtr), _currentlyUsedPtr(0)
        { }
        ~VolatilePtrThreadSnapshotFactory() { }
        
        // Const
        T* currentlyUsedPtr() const {
            return _counter.value() ? _currentlyUsedPtr : 0;
        }

        // Non-Const
        Observed<T*> createSnapshot() {
            _counter.increment();
            if( _counter.value() == 1 ) { // No real snapshots
                _currentlyUsedPtr = _volatileRawPtr.getPtr();
            }
            Observed<T*> snapshot(_currentlyUsedPtr, _counter);
            _counter.decrement();
            return snapshot;
        }
    private:
        VolatileRawPtr<T,MULTI_WRITERS>& _volatileRawPtr;
        T* _currentlyUsedPtr;
        RefCounter _counter;
    };
}

/***
 * Implementation
 ***/
template <typename T, bool MW>
promote::VolatilePtr<T,MW>::~VolatilePtr()
{
    ThreadNode* node(_headNode);
    while( node ) {
        ThreadNode* const tempNode(node);
        node = node->nextNode;
        delete tempNode;
    }
}

template <typename T, bool MW>
bool promote::VolatilePtr<T,MW>::isRawPtrReferenced(T* const ptr) const
{
    if( ptr != getRawPtr() ) {
        ThreadNode* node(_headNode);
        while( node ) {
            if( node->snapshotFactory->currentlyUsedPtr() == ptr ) {
                return true;
            } else {
                node = node->nextNode;
            }
        }
        return false;
    } else {
        return true;
    }
}

template <typename T, bool MW>
//typename promote::VolatilePtr<T,MW>::Snapshot promote::VolatilePtr<T,MW>::snapshot()
promote::Observed<T*> promote::VolatilePtr<T,MW>::snapshot()
{
    pthread_t const pthread(pthread_self());

    // Find existing ThreadNode for this pthread
    ThreadNode* const unlockedFirstHeadNode(_headNode);
    ThreadNode* node(findNodeForThread(pthread, unlockedFirstHeadNode, NULL));
    if( node == NULL ) {
        _newNodeMutex.lock();
        ThreadNode* const lockedSecondHeadNode(_headNode);
        // Make sure a ThreadNode for this node hasn't been added
        node = findNodeForThread(pthread, lockedSecondHeadNode, unlockedFirstHeadNode);
        if( node == unlockedFirstHeadNode ) {
            // Time to create a new ThreadNode
            _headNode = (node = new ThreadNode(pthread, _rawPtr, lockedSecondHeadNode));
        }
        _newNodeMutex.unlock();
    }

    // Create Snapshot from ThreadNode's SnapshotFactory
    return node->snapshotFactory->createSnapshot();
}

template <typename T, bool MW>
promote::VolatilePtr<T,MW>::ThreadNode::ThreadNode(pthread_t const pthread_,
                                                   promote::VolatileRawPtr<T,MW>& rawPtr_,
                                                   promote::VolatilePtr<T,MW>::ThreadNode* const nextNode_)
:   pthread(pthread_), nextNode(nextNode_), 
    snapshotFactory(new VolatilePtrThreadSnapshotFactory<T,MW>(rawPtr_))
{ }

template <typename T, bool MW>
promote::VolatilePtr<T,MW>::ThreadNode::~ThreadNode()
{
    delete snapshotFactory;
}

#endif /* PROMOTE_VOLATILE_PTR_HPP_ */

