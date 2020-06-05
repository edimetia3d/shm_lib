//
// Created on 6/2/20.
//

#ifndef SHM_STRUCT_SHARED_MEM_MUTEX_H
#define SHM_STRUCT_SHARED_MEM_MUTEX_H

#include <pthread.h>

class SharedMemMutex {
public:
    SharedMemMutex();

    ~SharedMemMutex();

    void lock();

    void unlock();

    void try_lock();

private:
    pthread_mutex_t mutex_;
    pthread_mutexattr_t mutex_attr_;
};


#endif //SHM_STRUCT_SHARED_MEM_MUTEX_H
