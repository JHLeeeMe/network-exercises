/** System V Semaphore
 *    union semun
 *    {
 *        int                 val;
 *        struct semid_ds*    buf;
 *        unsigned short int* array;
 *    #ifdef __linux__
 *        struct seminfo*     __buf;
 *    #endif
 *    } args;
 *
 *    void lock(int sem_id);
 *    void unlock(int sem_id);
 *    int  create_sem(int key, int sem_nums);
 *    void init_sem(int sem_id);
 *    void remove_sem(int sem_id);
 */
#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define __SEM_KEY  (1201)
#define __SEM_NUMS (1)


namespace jhleeeme
{
namespace sysv
{
namespace sem
{


union semun
{
    int                 val;
    struct semid_ds*    buf;
    unsigned short int* array;
#ifdef __linux__
    struct seminfo*     __buf;
#endif
} args;

void lock(int sem_id)
{
    printf("Trying to lock...\n");

    //struct sembuf p_buf = {0, -1, 0};
    struct sembuf p_buf;
    p_buf.sem_num = 0;
    p_buf.sem_op = -1;
    p_buf.sem_flg = 0;
    //p_buf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &p_buf, 1) < 0)
    {
        std::cerr << "p(): semop() failed..." << std::endl;
        exit(1);
    }

    printf("Locked.\n");
}

void unlock(int sem_id)
{
    printf("Trying to unlock...\n");

    //struct sembuf v_buf = {0, 1, 0};
    struct sembuf v_buf;
    v_buf.sem_num = 0;
    v_buf.sem_op = 1;
    v_buf.sem_flg = 0;
    //v_buf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &v_buf, 1) < 0)
    {
        std::cerr << "v(): semop() failed..." << std::endl;
        exit(1);
    }

    printf("Unlocked.\n");
}

int create_sem(int key, int sem_nums)
{
    int sem_id = semget(__SEM_KEY, __SEM_NUMS, IPC_CREAT | 0644);
    if (sem_id < 0)
    {
        std::cerr << "jhleeeme::sysv::sem::create_sem() failed..." << std::endl;
        exit(1);
    }

    return sem_id;
}

void init_sem(int sem_id)
{
    args.val = 1;
    if (semctl(sem_id, 0, SETVAL, args) < 0)
    {
        std::cerr << "jhleeeme::sysv::sem::init_sem() - semctl() failed..." << std::endl;
        exit(1);
    }
}

void remove_sem(int sem_id)
{
    if (semctl(sem_id, 0, IPC_RMID, args) < 0)
    {
        std::cerr << "jhleeeme::sysv::sem::remove_sem() - semctl() failed..." << std::endl;
        exit(1);
    }
}


}
}
}
