#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define __SEM_NUMS (1)


void p(int sem_id);
void v(int sem_id);

union semun
{
    int                 val;
    struct semid_ds*    buf;
    unsigned short int* array;
#ifdef __linux__
    struct seminfo*     __buf;
#endif
} args;

int main()
{
    // Create sem
    key_t key = ftok("./bin/", 12);
    int sem_id = semget(key, __SEM_NUMS, IPC_CREAT | 0644);

    // Init sem
    args.val = 1;
    if (semctl(sem_id, 0, SETVAL, args) < 0)
    {
        std::cerr << "main(): semctl() (for Init) failed..." << std::endl;
        exit(1);
    }

    // Lock & Unlock
    p(sem_id);
    v(sem_id);

    // Remove sem
    if (semctl(sem_id, 0, IPC_RMID, args) < 0)
    {
        std::cerr << "main(): semctl() (for Remove) failed..." << std::endl;
        exit(1);
    }

    return 0;
}

void p(int sem_id)
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

void v(int sem_id)
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
