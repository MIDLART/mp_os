#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <fstream>


#include <sys/msg.h>
#include <sys/unistd.h>


#define LINUX_SERVER_KEY 100
#define MAX_MSG_TEXT_SIZE 1024
#define SHUTDOWN_PRIOR 1
#define LOG_PRIOR 2


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

static int flag = 1;
static int mq_descr = -1;

std::string decode_severity(int code)
{
    switch (code)
    {
        case 0:
            return "TRACE";
        case 1:
            return "DEBUG";
        case 2:
            return "INFORMATION";
        case 3:
            return "WARNING";
        case 4:
            return "ERROR";
        case 5:
            return "CRITICAL";
        default:
            return "UNIDENTIFIED";
    }
}


void reader()
{
    msg_t msg;
    std::string cmd;

    while (std::cin >> cmd)
    {
        if (cmd == "shutdown")
        {
            msg.mtype = SHUTDOWN_PRIOR;

            msgsnd(mq_descr, &msg, sizeof(msg_t), 0);

            flag = 0;

            break;
        }
    }
}

int main()
{
    msg_t msg;
    size_t const max_msg_size = sizeof(msg_t);
    std::map<std::string, std::ofstream> streams;

    mq_descr = msgget(LINUX_SERVER_KEY, IPC_CREAT | 0666);

    if(mq_descr == -1)
    {
        std::cout << "Cannot open the message queue" << std::endl;
        return 1;
    }

    std::thread cmd_thread(reader);

    while (flag)
    {
        ssize_t rcv_cnt = msgrcv(mq_descr, &msg, max_msg_size, 0, MSG_NOERROR);
        if (rcv_cnt == -1)
        {
            std::cout << "Error receiving message" << std::endl;

            break;
        }

        if (msg.mtype == SHUTDOWN_PRIOR)
        {
            break;
        }

        if (msg.file_path[0] && !streams.count(msg.file_path))
        {
            std::ofstream fstream = std::ofstream(msg.file_path, std::ios::app);

            if (!fstream.is_open())
            {
                std::cerr << "Error opening file" << std::endl;
            }

            streams[msg.file_path] = std::move(fstream);
        }
    }

    for (auto &elem : streams)
    {
        std::ofstream &stream = elem.second;

        stream.flush();
        stream.close();
    }

    msgctl(mq_descr, IPC_RMID, nullptr);

    std::cout << "Server shut down" << std::endl;

    cmd_thread.detach();
}
