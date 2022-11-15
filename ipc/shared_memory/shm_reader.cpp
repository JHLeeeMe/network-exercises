#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

#define __SHM_SIZE (1024)


int main()
{
    key_t key = ftok("./bin/", 65);
    int shm_id = shmget(key, __SHM_SIZE, IPC_CREAT | 0644);

    // attach
    void* shm_addr = shmat(shm_id, (void*)0, 0);

    std::cout << "Shared Data: " << (char*)shm_addr << std::endl;

    // detach & destroy
    shmdt(shm_addr);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
