#ifndef PROMOTE_SHARED_VARIABLE_HPP_
#define PROMOTE_SHARED_VARIABLE_HPP_

#include <cassert>

#include "ArrayList.hpp"
#include "CacheLineAllocator.hpp"
#include "Utilities.hpp"

namespace promote
{
    template <typename T>
    class SharedVariable
    {
    public:
        class CachedView
        {
        public:
            CachedView(SharedVariable<T>* const sharedVar)
            :   _sharedVar(sharedVar)
            {
                get(); // To init cached version
            }

            /* Const */
            typename Passable<T const>::Type getCached() const { return _cached; }
            
            /* Non-Const */
            typename Passable<T const>::Type get() 
            {
                _cached = *(_sharedVar->get());
                return _cached;
            }
            void set(typename Passable<T const>::Type value) 
            { 
                _cached = value;
                _sharedVar->set(value);
            }
        private:
            SharedVariable<T>* const _sharedVar;
            T _cached;
        };

        SharedVariable();
        SharedVariable(typename Passable<T const>::Type value);
        ~SharedVariable();

        // Get + Set
        T volatile* get() const                             { return _value; }
        void set(typename Passable<T const>::Type value)    { *_value = value; }

        // CachedView Factory
        CachedView* createCachedView()
        {
            CachedView* const view(new(CacheLineAllocator::instance().alloc(sizeof(CachedView))) CachedView(this));
            _views.add(view);
            return view;
        }
        void destroyCachedView(CachedView* const view)
        {
            //_views.remove(view);
            for(typename ArrayList<CachedView*>::Iterator iter = _views.begin();
                iter != _views.end();
                ++iter)
            {
              if( *iter == view ) {
                *iter = 0;
                break;
              }
            }
            CacheLineAllocator::instance().release(view);
        }
    private:
        T volatile* _value;
        ArrayList<CachedView*> _views; 
    };
}

/***
 * Implementation
 ***/
template <typename T>
promote::SharedVariable<T>::SharedVariable()
:   _value(new(promote::CacheLineAllocator::instance().alloc(sizeof(T))) T),
    _views(2)
{ 
    assert(_value);
}

template <typename T>
promote::SharedVariable<T>::SharedVariable(typename Passable<T const>::Type value)
:   _value(new(promote::CacheLineAllocator::instance().alloc(sizeof(T))) T(value)),
    _views(2)
{
    assert(_value);
}

template <typename T>
promote::SharedVariable<T>::~SharedVariable()
{
    promote::CacheLineAllocator::instance().release(const_cast<T*>(_value));
    for(std::size_t ii = 0; ii != _views.size(); ++ii) {
        if( _views[ii] ) {
            promote::CacheLineAllocator::instance().release(_views[ii]);
        }
    }
}

#endif /* PROMOTE_SHARED_VARIABLE_HPP_ */

