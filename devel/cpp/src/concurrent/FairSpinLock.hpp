#ifndef PROMOTE_FAIR_SPIN_LOCK_HPP_
#define PROMOTE_FAIR_SPIN_LOCK_HPP_

#include "Atomic.hpp"

namespace promote
{
    class FairSpinLock
    {
    public:
        FairSpinLock();
        ~FairSpinLock();

        bool isContested()
        {
            return _owner.fetch() < _nextTicket.fetch() - 1;
        }

        void lock()
        {
            unsigned const ticket(_nextTicket.fetchAndAdd(1));
            while( _owner.fetch() != ticket ) ;
        }

        bool trylock()
        {
            unsigned const owner(_owner.fetch());
            return _nextTicket.compareAndSwap(owner, owner+1);
        }

        void unlock()
        {
            _owner.addAndFetch(1);
        }
    private:
        Atomic<unsigned> _owner;
        Atomic<unsigned> _nextTicket;
    };
};

#endif /* PROMOTE_FAIR_SPIN_LOCK_HPP_ */

