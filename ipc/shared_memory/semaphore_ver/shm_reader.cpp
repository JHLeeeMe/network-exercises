#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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

    int cnt = 0;
    while (true)
    {
        if (cnt++ == 30)
        {
            break;
        }
        jhleeeme::sysv::sem::lock(sem_id);

        std::cout << "Reader: " << (char*)shm_addr << std::endl;

        jhleeeme::sysv::sem::unlock(sem_id);
        sleep(1);
    }

    // detach & destroy
    shmdt(shm_addr);
    shmctl(shm_id, IPC_RMID, NULL);

    jhleeeme::sysv::sem::remove_sem(sem_id);

    return 0;
}
