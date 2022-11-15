/** System V Message Queue Receiver
 *    ftok(): is use to generate a unique key.
 *    msgget(): either returns the message queue identifier for a newly created message queue or returns the identifiers for a queue which exists with the same key value.
 *    msgsnd(): Data is placed on to a message queue by calling msgsnd().
 *    msgrcv(): messages are retrieved from a queue.
 *    msgctl(): It performs various operations on a queue. Generally it is use to destroy message queue.
 */
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>


struct Data
{
    char data[32];
};

struct MsgBuf
{
    long        msg_type;
    struct Data payload;
};


int main()
{
    std::cout << "-------------" << std::endl;
    std::cout << "Sender start!" << std::endl;
    std::cout << "-------------" << std::endl;

    key_t key = ftok("/dev/mqueue/", 65);
    int msg_id = msgget(key, IPC_CREAT | 0666);
    if (msg_id < 0)
    {
        std::cerr << "msgget() failed..." << std::endl;
        exit(1);
    }

    Data payload;
    MsgBuf msg;

    int cnt = 0;
    while (cnt < 10)
    {
        memset(&payload, 0x00, 32);
        strcpy(payload.data, "hello, ipc!");

        memset(&msg, 0x00, sizeof(msg));
        msg.msg_type = cnt % 2 == 0 ? 2 : 1;
        msg.payload = payload;

        //if (msgsnd(msg_id, &msg, sizeof(struct Data), IPC_NOWAIT) < 0)
        if (msgsnd(msg_id, &msg, sizeof(struct Data), 0) < 0)
        {
            std::cerr << "msgsnd() failed..." << std::endl;
            exit(1);
        }

        std::cout << "Send data: " << payload.data << std::endl;

        ++cnt;
        sleep(1);
    }

    struct msqid_ds mqueue_stat;
    while (true)
    {
        if (msgctl(msg_id, IPC_STAT, &mqueue_stat) < 0)
        {
            std::cerr << "msgctl(..., IPC_STAT, ...) failed..." << std::endl;
            exit(1);
        }

        if (mqueue_stat.msg_qnum == 0)
        {
            break;
        }

        sleep(1);
    }

    // rm message queue
    if (msgctl(msg_id, IPC_RMID, NULL) < 0)
    {
        std::cerr << "msgctl() failed..." << std::endl;
        exit(1);
    }

    std::cout << "-----------" << std::endl;
    std::cout << "Sender end!" << std::endl;
    std::cout << "-----------" << std::endl;

    return 0;
}
