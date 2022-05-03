// rose从消息队列中读消息
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

    struct T_msgbuf msgbuf;

    bzero(&msgbuf, sizeof(msgbuf));

    for(int i=0; i<10; i++)
    {
        // 接收消息函数
        if(-1==msgrcv(msgid, &msgbuf, MSG_SIZE, J2R, 0))
        {
            perror("msgrcv()");
            exit(1);
        }
        printf("from msg: %s\n", msgbuf.msg_text);
    }
    // 删除消息队列
    msgctl(msgid, IPC_RMID, NULL);
    printf("delete msgid！\n");
    return 0;
}

