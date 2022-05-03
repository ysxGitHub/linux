// jack向消息队列中发送消息
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include <signal.h>

#define MSG_SIZE 64   // 单个消息的最大字节数
#define PROJ_PATH "." // 使用当前路径来产生消息队列的key值
#define PROJ_ID 1

#define J2R 1L // jack发送给rose的消息标识
#define R2J 2L // rose发送给jack的消息标识

struct T_msgbuf
{ // 带标识的消息结构体
    long msg_tpye;
    char msg_text[MSG_SIZE];
};

int main(int argc, char const *argv[])
{
    // 获得key
    key_t key = ftok(PROJ_PATH, PROJ_ID);

    // 获得消息队列ID
    int msgid = msgget(key, IPC_CREAT | 0666);

    struct T_msgbuf message1;
    struct T_msgbuf message2;

    bzero(&message1, sizeof(message1));
    bzero(&message2, sizeof(message2));

    // 指定消息标识
    message1.msg_tpye = J2R;
    // 按照长度拷贝
    strncpy(message1.msg_text, "I love you, Rose!\n", MSG_SIZE);

    message2.msg_tpye = 8L;
    strncpy(message2.msg_text, "Other message!!!\n", MSG_SIZE);

    for (int i = 0; i < 10; i++)
    {
        // 发送消息函数
        int res1 = msgsnd(msgid, &message1, strlen(message1.msg_text), 0);
        int res2 = msgsnd(msgid, &message2, strlen(message2.msg_text), 0);

        if ((res1 != 0) || (res2 != 0))
        {
            perror("msgsend() error");
            exit(1);
        }
    }
    printf("message send over！\n");
    return 0;
}
