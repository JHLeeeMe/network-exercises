#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

#define __SHM_SIZE (1024)


int main()
{
    key_t key = ftok("./bin/", 65);
    int shm_id = shmget(key, __SHM_SIZE, IPC_CREAT | 0644);

    // attach
    char* shared_memory = (char*)shmat(shm_id, (void*)0, 0);

    std::cout << "Shared Data: " << shared_memory << std::endl;

    // detach & destroy
    shmdt(shared_memory);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
