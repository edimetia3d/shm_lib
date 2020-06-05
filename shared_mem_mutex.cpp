//
// Created on 6/2/20.
//

#include "shared_mem_mutex.h"

SharedMemMutex::SharedMemMutex() {
    pthread_mutexattr_init(&mutex_attr_);
    pthread_mutexattr_setrobust(&mutex_attr_, PTHREAD_MUTEX_ROBUST); // single process may get killed individually
    pthread_mutexattr_setpshared(&mutex_attr_, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mutex_, &mutex_attr_);
}

void SharedMemMutex::lock() {
    pthread_mutex_lock(&mutex_);
}

void SharedMemMutex::unlock() {
    pthread_mutex_unlock(&mutex_);
}

void SharedMemMutex::try_lock() {
    pthread_mutex_trylock(&mutex_);
}

SharedMemMutex::~SharedMemMutex() {
    pthread_mutex_destroy(&mutex_);
    pthread_mutexattr_destroy(&mutex_attr_);
}
