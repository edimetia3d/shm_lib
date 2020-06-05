//
// Created on 6/2/20.
//

#include <mutex>
#include <unistd.h>
#include "../shm_array.h"

int main() {
    ShmArray arr;
    ShmArray::Get("test", &arr);

    int32_t *intv = static_cast<int *>(arr.data);
    int next = 1;
    while (next < 100) {
        while (intv[0] != next) {
            usleep(100);
        }
        std::unique_lock<SharedMemMutex> ulock(*arr.mutex);
        ++intv[0];
        next = intv[0] + 1;
        printf("Goto %d \n", intv[0]);
    }

    arr.Close();

}