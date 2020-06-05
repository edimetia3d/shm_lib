
#include "../shm_array.h"

int main() {
    ShmArray tmp;
    ShmArray::Create("test", 1024, &tmp);
    tmp.Close();
    return 0;
}