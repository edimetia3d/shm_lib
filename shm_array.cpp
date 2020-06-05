//
// Created on 6/2/20.
//

#include "shm_array.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define HEAD_ALIGNED_SIZE 1024
struct HeadShmArray_Raw {
    int data_mem_size;
    SharedMemMutex mutex;
};

struct HeadShmArray : public HeadShmArray_Raw {
    HeadShmArray(int data_mem) { data_mem_size = data_mem; }

    uint8_t reserved[HEAD_ALIGNED_SIZE - sizeof(HeadShmArray_Raw)];
};

static std::string ConvertKeyToPath(const std::string &key) {
    return std::string("/") + key;
}

ShmArrayError ShmArray::Create(const std::string &key, int mem_size, ShmArray *ret) {
    std::string path = ConvertKeyToPath(key);
    ret->fd = shm_open(path.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (ret->fd == -1) {
        return ShmArrayError::SHM_OPEN_FAILED;
    }
    ret->real_size = mem_size + (int) sizeof(HeadShmArray);
    ret->mem_size = mem_size;
    int err = ftruncate(ret->fd, ret->real_size);
    if (err == -1) {
        return ShmArrayError::TRUNCATE_FAILD;
    }

    void *buf = mmap(nullptr, ret->real_size, PROT_WRITE | PROT_READ, MAP_SHARED, ret->fd, 0);
    if (buf == MAP_FAILED) {
        return ShmArrayError::MMAP_FAILED;
    }

    new(buf) HeadShmArray(mem_size);
    HeadShmArray *head = reinterpret_cast<HeadShmArray *>(buf);

    ret->data = reinterpret_cast<uint8_t *>(buf) + sizeof(HeadShmArray);
    ret->mutex = &head->mutex;
    return ShmArrayError::NO_ERROR;
}

void ShmArray::Close() {
    munmap(data, real_size);
    close(fd);
    *this = ShmArray();
}

ShmArrayError ShmArray::Destroy(const std::string &key) {
    std::string path = ConvertKeyToPath(key);
    shm_unlink(path.c_str());
    return ShmArrayError::NO_ERROR;
}

ShmArrayError ShmArray::Get(const std::string &key, ShmArray *ret) {
    std::string path = ConvertKeyToPath(key);
    ret->fd = shm_open(path.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if (ret->fd == -1) {
        return ShmArrayError::SHM_OPEN_FAILED;
    }

    void *buf = mmap(nullptr, sizeof(HeadShmArray), PROT_WRITE | PROT_READ, MAP_SHARED, ret->fd, 0);
    if (buf == MAP_FAILED) {
        return ShmArrayError::MMAP_FAILED;
    }
    HeadShmArray *head = reinterpret_cast<HeadShmArray *>(buf);
    ret->real_size = head->data_mem_size + sizeof(HeadShmArray);
    ret->mem_size = head->data_mem_size;
    munmap(buf, sizeof(HeadShmArray));

    buf = mmap(nullptr, ret->real_size, PROT_WRITE | PROT_READ, MAP_SHARED, ret->fd, 0);
    if (buf == MAP_FAILED) {
        return ShmArrayError::MMAP_FAILED;
    }
    head = reinterpret_cast<HeadShmArray *>(buf);
    ret->mutex = &head->mutex;
    ret->data = reinterpret_cast<uint8_t *>(buf) + sizeof(HeadShmArray);
    return ShmArrayError::NO_ERROR;
}

