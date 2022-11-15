#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include "semaphore.h"

#define __SHM_SIZE (1024)
#define __SEM_KEY  (1201)
#define __SEM_NUMS (1)


int main()
{
    int sem_id = jhleeeme::sysv::sem::create_sem(__SEM_KEY, __SEM_NUMS);
    jhleeeme::sysv::sem::init_sem(sem_id);
    
    key_t key = ftok("./bin/", 65);
    int shm_id = shmget(key, __SHM_SIZE, IPC_CREAT | 0644);

    // attach
    void* shm_addr = shmat(shm_id, (void*)0, 0);
    if (shm_addr == (void*)-1)
    {
        std::cerr << "shmat() failed..." << std::endl;
        exit(1);
    }

    int cnt = 0;
    while (cnt != 5)
    {
        jhleeeme::sysv::sem::lock(sem_id);

        sprintf((char*)shm_addr, "%d", cnt++);
        std::cout << "Writer: " << (char*)shm_addr << std::endl;

        jhleeeme::sysv::sem::unlock(sem_id);
        sleep(2);
    }

    // detach
    shmdt(shm_addr);

    return 0;
}
