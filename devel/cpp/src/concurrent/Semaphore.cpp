
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
#include <mach/mach_init.h>
#endif

#include "Semaphore.hpp"

using namespace promote;

Semaphore::Semaphore(int initValue)
{
#ifndef __APPLE__
    if( sem_init(&_underlying, 0, initValue) ) {
#else
    if( semaphore_create(mach_task_self(), &_machSemaphore, SYNC_POLICY_FIFO, initValue) ) {
#endif
        throw Exception("Semaphore init failed."); //TODO: real error
    }
}

Semaphore::~Semaphore()
{
#ifndef __APPLE__
    if( sem_destroy(&_underlying) ) {
#else
    if( semaphore_destroy(mach_task_self(), _machSemaphore) ) {
#endif
        throw Exception("Semaphore uninit failed."); //TODO: real error
    }
}

