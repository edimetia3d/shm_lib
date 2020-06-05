//
// Created on 6/2/20.
//

#ifndef SHM_STRUCT_SHM_ARRAY_H
#define SHM_STRUCT_SHM_ARRAY_H

#include <string>

#include "shared_mem_mutex.h"

enum ShmArrayError {
    NO_ERROR,
    SHM_OPEN_FAILED,
    TRUNCATE_FAILD,
    MMAP_FAILED
};

/**
 * ShmArray's Create and Destroy is not Process-safe, user must control the order of calling
 */
struct ShmArray {
    static ShmArrayError Get(const std::string &key, ShmArray *ret);

    static ShmArrayError Create(const std::string &key, int mem_size, ShmArray *ret);

    static ShmArrayError Destroy(const std::string &key);

    void Close();

    int mem_size = -1;
    void *data = nullptr;
    SharedMemMutex *mutex;
private:
    int real_size = -1;
    int fd = -1;
};


#endif //SHM_STRUCT_SHM_ARRAY_H
