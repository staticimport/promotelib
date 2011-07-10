
#include "Mutex.hpp"

using namespace promote;

Mutex::Mutex()
{
    if( pthread_mutex_init(&_mutex,0) != 0 ) {
        throw Exception("Failed to init Mutex."); // TODO: give error
    }
}

Mutex::~Mutex()
{
    if( pthread_mutex_destroy(&_mutex) != 0 ) {
        throw Exception("Failed to uninit Mutex."); // TODO: give error
    }
}

