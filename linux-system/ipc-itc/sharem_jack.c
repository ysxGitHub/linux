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

int shmid;


// 信号来了，执行删除共享内存ID的动作
void rmid(int sig)
{
    shmctl(shmid, IPC_RMID, NULL);
}

int main(int argc, char const *argv[])
{   
    // 捕捉一个信号，捕捉后的动作为执行函数指针指向的函数
    signal(SIGINT, rmid);
    // 获得IPC的key
    key_t key = ftok(PROJ_PATH, PROJ_ID);
    // 获得共享内存ID
    shmid = shmget(key, SHMSZ, IPC_CREAT|0666);
    // 将该共享内存映射到当前进程中，如果第二个参数为NULL，则系统指定返回地址
    char*p = (char*)shmat(shmid, NULL, 0);
    // 
    bzero(p, SHMSZ);
    // Jack将自身的PID信息放入到SHM的前4个字节
    pid_t pid = getpid();
    // 
    printf("Current pid=%d\n", pid);
    // 
    memcpy(p, &pid, sizeof(pid_t));
    // 从键盘输入数据到共享内存中
    fgets(p+sizeof(pid_t), (SHMSZ-sizeof(pid_t)), stdin);
    // 等待rose的信号去删除SHM
    pause();
    // 解除共享内存映射
    shmdt(p);
    return 0;
}



