#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define __SHM_SIZE (1024)


int main()
{
    key_t key = ftok("./bin/", 65);
    int shm_id = shmget(key, __SHM_SIZE, IPC_CREAT | 0644);

    // attach
    void* shm_addr = shmat(shm_id, (void*)0, 0);
    if (shm_addr == (void*)-1)
    {
        std::cerr << "shmat() failed..." << std::endl;
        exit(1);
    }

    const char* tmp = "Hello, shm!";
    memcpy(shm_addr, tmp, strlen(tmp));
    std::cout << "Shared Data: " << (char*)shm_addr << std::endl;

    // detach
    shmdt(shm_addr);

    return 0;
}
