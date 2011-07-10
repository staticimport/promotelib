
#include "FairSpinLock.hpp"

using namespace promote;

FairSpinLock::FairSpinLock()
:   _owner(0),
    _nextTicket(0)
{ }

FairSpinLock::~FairSpinLock() { }

