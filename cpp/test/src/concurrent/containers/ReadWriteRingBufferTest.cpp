
#include <iostream>
#include <pthread.h>

#include "Atomic.hpp"
#include "ReadWriteRingBuffer.hpp"
#include "ReadWriteRingBufferTest.hpp"
#include "StopWatch.hpp"
#include "Testing.hpp"

using namespace promote;

struct Entry {
    static Atomic<std::size_t> NEXT_ID;
    
    Entry() : id(NEXT_ID.fetchAndAdd(1)) { }
    Entry(Entry const& e) : id(e.id) { }

    std::size_t const id;
};

Atomic<std::size_t> Entry::NEXT_ID(0);

static std::string testPeek();
static std::string testPop();
static std::string testPush();

void testing::perfTestReadWriteRingBuffer() {
    std::cout << "[ ReadWriteRingBuffer ]\n";
}

void testing::unitTestReadWriteRingBuffer() {
    std::cout << "[ ReadWriteRingBuffer ]\n";
    testing::printUnitResult("peek", testPeek());
    testing::printUnitResult("pop", testPop());
    testing::printUnitResult("push", testPush());
}

static std::string testPeek() {
    Entry::NEXT_ID.set(0);
    ReadWriteRingBuffer<Entry> buffer(1000);
    for(std::size_t ii = 0; ii != buffer.capacity(); ++ii) {
        buffer.push(Entry());
        Entry* const frontPtr(buffer.tryPeek());
        if( frontPtr == NULL ) {
            return "TryPeek wrongly returned NULL. (1)";
        } else if( frontPtr->id != 0 ) {
            return "TryPeek returned wrong value. (2)";
        }
        try {
            if( buffer.peek().id != 0 ) {
                return "Non-Const Peek returned wrong value. (3)";
            }
        } catch(...) {
            return "Non-Const Peek wrongly threw exception. (4)";
        }
        try {
            if( const_cast<ReadWriteRingBuffer<Entry> const*>(&buffer)->peek().id != 0 ) {
                return "Const peek returned wrong value. (5)";
            }
        } catch(...) {
            return "Const peek wrongly threw exception. (6)";
        }
    }
    for(std::size_t ii = 0; ii != buffer.capacity(); ++ii) {
        Entry* const frontPtr(buffer.tryPeek());
        if( frontPtr == NULL ) {
            return "TryPeek wrongly returned NULL. (8)";
        } else if( frontPtr->id != ii ) {
            return "TryPeek returned wrong value. (9)";
        }
        try {
            if( buffer.peek().id != ii ) {
                return "Non-Const Peek returned wrong value. (10)";
            }
        } catch(...) {
            return "Non-Const Peek wrongly threw exception. (11)";
        }
        try {
            if( const_cast<ReadWriteRingBuffer<Entry> const*>(&buffer)->peek().id != ii ) {
                return "Const peek returned wrong value. (12)";
            }
        } catch(...) {
            return "Const peek wrongly threw exception. (13)";
        }
        try {
            buffer.pop();
        } catch(...) {
            return "Exception thrown during pop. (7)";
        }
    }
    if( buffer.tryPeek() ) {
        return "TryPeek wrongly returned non-NULL. (14)";
    }
    bool exceptionThrown(false);
    try {
        buffer.peek();
    } catch(...) {
        exceptionThrown = true;
    }
    if( !exceptionThrown ) {
        return "Non-Const peek wrongly didn't throw exception. (15)";
    }
    exceptionThrown = false;
    try {
        const_cast<ReadWriteRingBuffer<Entry> const*>(&buffer)->peek();
    } catch(...) {
        exceptionThrown = true;
    }
    if( !exceptionThrown ) {
        return "Const peek wrongly didn't throw exception. (16)";
    }
    return "";
}

static std::string testPop() {
    return "Not yet implemented.";
}

static std::string testPush() {
    ReadWriteRingBuffer<Entry> buffer(1000);
    for(std::size_t ii = 0; ii != buffer.capacity(); ++ii) {
        if( buffer.full(false) ) {
            return "full(false) wrongly returned true.";
        } else if( buffer.full(true) ) {
            return "full(true) wrongly returned true.";
        } else if( buffer.size() != ii ) {
            return "size incorrect";
        }
        try {
            buffer.push(Entry());
        } catch(...) {
            return "push on non-full buffer threw exception.";
        }
        if( buffer.empty(false) ) {
            return "empty(false) wrongly returned true.";
        } else if( buffer.empty(true) ) {
            return "empty(true) wrongly returned true.";
        }
    }
    if( !buffer.full(false) ) {
        return "full(false) wrongly returned false.";
    } else if( !buffer.full(true) ) {
        return "full(true) wrongly returned false.";
    }
    try {
        buffer.push(Entry());
    } catch(...) {
        return "";
    }
    return "push on full buffer didn't throw exception.";
}


