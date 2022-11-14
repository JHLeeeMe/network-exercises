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

    int msg_type;
    msg_type = argc == 2 ? 1 : 2;

    MsgBuf msg;

    while (true)
    {
        if (msg_type == 1)
        {
            std::cout << "msg_type: " << msg_type << std::endl;
        }
        else
        {
            std::cout << "msg_type: " << msg_type << std::endl;
        }

        if (msgrcv(msg_id, &msg, sizeof(msg.payload), msg_type, 0) < 0)
        {
            std::cerr << "msgrcv() failed..." << std::endl;
            exit(1);
        }
        std::cout << msg.payload.data << std::endl;
    }

    return 0;
}
