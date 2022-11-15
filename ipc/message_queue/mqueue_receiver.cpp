/** System V Message Queue Receiver
 *    ftok(): is use to generate a unique key.
 *    msgget(): either returns the message queue identifier for a newly created message queue or returns the identifiers for a queue which exists with the same key value.
 *    msgsnd(): Data is placed on to a message queue by calling msgsnd().
 *    msgrcv(): messages are retrieved from a queue.
 *    msgctl(): It performs various operations on a queue. Generally it is use to destroy message queue.
 */
#include <iostream>
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

int main(int argc, char** argv)
{
    std::cout << "---------------" << std::endl;
    std::cout << "Receiver start!" << std::endl;
    std::cout << "---------------" << std::endl;

    key_t key = ftok("/dev/mqueue/", 65);
    int msg_id = msgget(key, IPC_CREAT | 0666);
    if (msg_id < 0)
    {
        std::cerr << "msgget() failed..." << std::endl;
        exit(1);
    }

    MsgBuf msg;
    while (true)
    {
        if (msgrcv(msg_id, &msg, sizeof(msg.payload), 0, 0) < 0)
        {
            std::cerr << "msgrcv() failed..." << std::endl;
            exit(1);
        }
        std::cout << "msg type: " << msg.msg_type << std::endl
                  << "msg     : " << msg.payload.data << std::endl;
    }

    std::cout << "-------------" << std::endl;
    std::cout << "Receiver end!" << std::endl;
    std::cout << "-------------" << std::endl;

    return 0;
}
