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
    char* shared_memory = (char*)shmat(shm_id, (void*)0, 0);
    if (shared_memory == (char*)-1)
    {
        std::cerr << "shmat() failed..." << std::endl;
        exit(1);
    }

    const char* tmp = "Hello, shm!";
    strncpy(shared_memory, "Hello, shm!", strlen(tmp));
    std::cout << "Shared Data: " << shared_memory << std::endl;

    // detach
    shmdt(shared_memory);

    return 0;
}
