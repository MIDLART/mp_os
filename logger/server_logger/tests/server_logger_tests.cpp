#include <string>
#include <sys/msg.h>
#include <sys/unistd.h>

#include <gtest/gtest.h>

#define LINUX_SERVER_KEY 100
#define MAX_MSG_TEXT_SIZE 1024
#define MAIN_PRIOR 1

struct msg_t
{
    long mtype;
    pid_t pid;
    size_t packed_id;
    size_t packed_cnt;
    char file_path[256];
    int severity;
    char mtext[MAX_MSG_TEXT_SIZE];
};

int main(
    int argc,
    char *argv[])
{
    int mq_descr = msgget(LINUX_SERVER_KEY, 0666);

    if(mq_descr == -1)
    {
        std::cout << "Cannot open the message queue" << std::endl;
    }

    msg_t msg;
    size_t const msg_max_size = sizeof(msg_t) - sizeof(long);

    msg.mtype = MAIN_PRIOR;
    msg.pid = getpid();
    msg.packed_id = 1;
    msg.packed_cnt = 1;
    msg.severity = 2;

    strcpy(msg.file_path, "");
    strcpy(msg., "123");

    msgsnd(mq_descr, &msg, msg_max_size, 0);
}