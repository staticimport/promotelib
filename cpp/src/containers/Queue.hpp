#ifndef PROMOTE_QUEUE_HPP_
#define PROMOTE_QUEUE_HPP_

#include <algorithm>
#include <cassert>
#include <limits>

#include "Exception.hpp"
#include "Power.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename T, bool fixed, bool rwsafe> class Queue;

    namespace queue {
        template <typename T, bool fixed=false, bool rwsafe=false>
        struct QueueVariables
        {
            QueueVariables(std::size_t minInitCapacity);
            ~QueueVariables();

            Queue<T,true,rwsafe>* readQueue;
            Queue<T,true,rwsafe>* writeQueue;
            Queue<Queue<T,true,rwsafe>*,true,rwsafe>* queues;
        };

        template <typename T>
        struct QueueVariables<T,false,true>
        {
            QueueVariables(std::size_t minInitCapacity);
            ~QueueVariables();

            Queue<T,true,true>* volatile readQueue;
            char space1[128];
            Queue<T,true,true>* volatile writeQueue;
            char space2[128];
            Queue<Queue<T,true,true>*,true,true>* volatile queues;
        };

        template <typename T>
        struct QueueVariables<T,true,false>
        {
            QueueVariables() : read(0), write(0) { }
            ~QueueVariables() { }

            std::size_t read;
            std::size_t write;
        };

        template <typename T>
        struct QueueVariables<T,true,true>
        {
            QueueVariables() : read(0), write(0), readLocalWrite(0), writeLocalRead(0) { }
            ~QueueVariables() { }

            volatile std::size_t read;
            char space1[128];
            volatile std::size_t write;
            char space2[128];
            std::size_t readLocalWrite;
            std::size_t writeLocalRead;
        };
    }

    template <typename T, bool fixed=false, bool rwsafe=false>
    class Queue
    {
    public:
        struct Exception : public promote::Exception {
            Exception(String const& reason) : promote::Exception(reason) { }
        };

        Queue(const std::size_t minInitCapacity) : _vars(minInitCapacity)
        {
            //_vars.readQueue = new Queue<T,true,rwsafe>(minInitCapacity);
            //_vars.writeQueue = _vars.readQueue;
            //_vars.queues = new Queue<Queue<T,true,rwsafe>*,true,rwsafe>(sizeof(std::size_t) << 3);
        }

        ~Queue()
        {
            delete _vars.readQueue;
            Queue<T,true,rwsafe>* queue;
            while( _vars.queues->read(queue) ) {
                delete queue;
            }
            delete _vars.queues;
        }

        /**
         * Observe
         **/
        std::size_t capacity() const
        {
            return std::numeric_limits<std::size_t>::max();
        }
        bool isEmpty() const
        {
            return _vars.readQueue->isEmpty() and _vars.queues->isEmpty();
        }
        bool isFull() const
        {
            return false;
        }
        typename Passable<T>::Type const peek()
        {
            if( !_vars.readQueue->isEmpty() ) {
                return _vars.readQueue->peek();
            } else if( !_vars.queues->isEmpty() ) {
                if( _vars.readQueue->isEmpty() ) {
                    delete _vars.readQueue;
                    _vars.queues->read(_vars.readQueue);
                }
                return _vars.readQueue->peek();
            } else {
                throw Exception("Attempted Peek in empty Queue.");
            }
        }
        std::size_t size() const
        {
            std::size_t size = _vars.readQueue->size();
            if( !_vars.queues->isEmpty() ) {
                const std::size_t readQueueCapacity = _vars.readQueue->capacity();
                const std::size_t queuesSize = _vars.queues->size();
                for(std::size_t ii = 1; ii < queuesSize; ++ii) {
                    size += (readQueueCapacity << ii);
                }
                size += _vars.writeQueue->size();
            }
            return size;
        }

        /**
         * Read
         **/
        T* beginRead()
        {
            T* item = _vars.readQueue->beginRead();
            if( item == 0 and !_vars.queues->isEmpty() ) {
                if( _vars.readQueue->isEmpty() ) {
                    delete _vars.readQueue;
                    _vars.queues->read(const_cast<Queue<T,true,rwsafe>*&>(_vars.readQueue));
                }
                item = _vars.readQueue->beginRead();
            }
            return item;
        }
        void endRead()
        {
            _vars.readQueue->endRead();
        }
        bool read(T& value)
        {
            if( _vars.readQueue->read(value) ) {
                //printf("old read\n");
                return true;
            } else if( !_vars.queues->isEmpty() ) {
                if( _vars.readQueue->isEmpty() ) {
                    delete _vars.readQueue;
                    Queue<T,true,rwsafe>* newReadQueue(0);
                    _vars.queues->read(newReadQueue);
                    //printf("New ReadQueue=%p of capacity=%d\n", newReadQueue, (int)newReadQueue->capacity());
                    _vars.readQueue = newReadQueue;
                }
                return _vars.readQueue->read(value);
            } else {
                return false;
            }
        }

        /**
         * Write
         **/
        T* beginWrite()
        {
            T* item = _vars.writeQueue->beginWrite();
            if( item ) {
                return item;
            } else {
                Queue<T,true,rwsafe>* newWriteQueue =
                    new Queue<T,true,rwsafe>(_vars.writeQueue->capacity() << 1);
                if( newWriteQueue and _vars.queues->write(newWriteQueue) ) {
                    _vars.writeQueue = newWriteQueue;
                    return _vars.writeQueue->beginWrite();
                } else {
                    return 0;
                }
            }
        }
        void endWrite()
        {
            _vars.writeQueue->endWrite();
        }
        bool write(typename Passable<T const>::Type value)
        {
            if( _vars.writeQueue->write(value) ) {
                //printf("old write\n");
                return true;
            } else {
                Queue<T,true,rwsafe>* newWriteQueue =
                    new Queue<T,true,rwsafe>(_vars.writeQueue->capacity() << 1);
                if( newWriteQueue ) {
                    //printf("New WriteQueue: %p\n of capacity: %d.\n", newWriteQueue, (int)newWriteQueue->capacity());
                    //printf("Adding value: %llu\n", (unsigned long long)value);
                    bool const result = newWriteQueue->write(value);
                    _vars.writeQueue = newWriteQueue;
                    assert(_vars.queues->write(newWriteQueue));
                    return result;
                } else {
                    return false;
                }
            }
        }

    private:
        queue::QueueVariables<T,false,rwsafe> _vars;
    };

    template <typename T, bool rwsafe>
    class Queue<T,true,rwsafe>
    {
    public:
        Queue(std::size_t minCapacity)
        : _capacity(static_cast<std::size_t>(promote::leastPower2Above64(static_cast<uint64_t>(minCapacity),true))),
          _mask(_capacity - 1),
          //_array(static_cast<T*>(::operator new(_capacity * sizeof(T))))
          _array(new T[_capacity])
        {
            _vars.read = _vars.write = 0;
        }

        ~Queue()
        {
            //::operator delete(_array);
            delete [] _array;
        }

        /**
         * Observe
         **/
        std::size_t capacity() const    { return _capacity; }
        bool isEmpty() const            { return _vars.write == _vars.read; }
        bool isFull() const             { return _vars.write == _vars.read + _capacity; }
        typename Passable<T const>::Type peek()
        {
            const std::size_t read(_vars.read);
            if( read != _vars.write ) {
                return _array[read & _mask];
            } else {
                throw Exception("Attempted Peek from empty Queue.");
            }
        }
        std::size_t size() const
        { 
            return std::max<std::size_t>(0, _vars.write - _vars.read);
        }

        /**
         * Read
         **/
        T* beginRead()
        {
            const std::size_t read = _vars.read;
            return read != _vars.write ? &(_array[read & _mask]) : 0;
        }
        void endRead()
        {
            ++_vars.read;
        }
        bool read(T& value)
        {
            const std::size_t read = _vars.read;
            if( read != _vars.write ) {
                value = _array[read & _mask];
                _vars.read = read + 1;
                return true;
            } else {
                return false;
            }
        }
        
        /**
         * Write
         **/
        T* beginWrite()
        {
            const std::size_t write(_vars.write);
            return write != _vars.read + _capacity ? &(_array[write & _mask]) : 0;
        }
        void endWrite()
        {
            ++_vars.write;
        }
        bool write(typename Passable<T const>::Type value)
        {
            const std::size_t write(_vars.write);
            if( write != _vars.read + _capacity ) {
                _array[write & _mask] = value;
                //new(&_array[write & _mask]) T(value);
                _vars.write = write + 1;
                return true;
            } else {
                return false;
            }
        }

    private:
        const std::size_t _capacity;
        const std::size_t _mask;
        T* const _array;
        queue::QueueVariables<T,true,rwsafe> _vars;
    };
}


/**
 * Implementation
 **/
//template <typename T, bool rwsafe>
//promote::QueueVariables<T,false,rwsafe>::QueueVariables(std::size_t minInitCapacity)
template <typename T, bool fixed, bool rwsafe>
promote::queue::QueueVariables<T,fixed,rwsafe>::QueueVariables(std::size_t minInitCapacity)
:   readQueue(new Queue<T,true,rwsafe>(minInitCapacity)),
    writeQueue(readQueue),
    queues(new Queue<Queue<T,true,rwsafe>*,true,rwsafe>(sizeof(std::size_t) << 3))
{
}

template <typename T>
promote::queue::QueueVariables<T,false,true>::QueueVariables(std::size_t minInitCapacity)
:   readQueue(new Queue<T,true,true>(minInitCapacity)),
    writeQueue(readQueue),
    queues(new Queue<Queue<T,true,true>*,true,true>(sizeof(std::size_t) << 3))
{
}

template <typename T, bool fixed, bool rwsafe>
promote::queue::QueueVariables<T,fixed,rwsafe>::~QueueVariables()
{
    //TODO
}

template <typename T>
promote::queue::QueueVariables<T,false,true>::~QueueVariables()
{
}

#endif /* PROMOTE_QUEUE_HPP_ */

