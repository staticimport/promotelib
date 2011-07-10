
#include "ReadWriteMutex.hpp"

using namespace promote;

ReadWriteMutex::ReadWriteMutex()
{
    if( pthread_rwlock_init(&_rwmutex,0) ) {
        throw Exception("Failed to init ReadWriteMutex.");
    }
}

ReadWriteMutex::~ReadWriteMutex()
{
    if( pthread_rwlock_destroy(&_rwmutex) ) {
        throw Exception("Failed to uninit ReadWriteMutex.");
    }
}

