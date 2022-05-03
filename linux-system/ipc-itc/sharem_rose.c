#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ 1024
#define PROJ_PATH "."
#define PROJ_ID 33

int main(int argc, char const *argv[])
{
    // 获得IPC的key
    key_t key = ftok(PROJ_PATH, PROJ_ID);
    // 获得共享内存ID
    int shmid = shmget(key, SHMSZ, 0666);
    // 将该共享内存映射到当前进程中，如果第二个参数为NULL，则系统指定返回地址
    char*p = (char*)shmat(shmid, NULL, 0);
    // 从共享内存中读取Jack发来的消息。
    printf("From Jack:%s\n", p+sizeof(pid_t));
    // 获得jack的进程PID
    pid_t jack_pid = *((pid_t*)(p));
    // 给Jack发送信号
    kill(jack_pid, SIGINT);
    //解除映射
    shmdt(p); 

    return 0;
}


