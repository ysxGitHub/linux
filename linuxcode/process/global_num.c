// number.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// 定义全局变量
int number = 10;

int main()
{
    printf("创建子进程之前 number = %d\n", number);

    pid_t pid = fork();
    // 父子进程都会执行这一行
    printf("当前进程fork()的返回值: %d\n", pid);

    //如果是父进程
    if(pid > 0)
    {
        printf("我是父进程, pid = %d, number = %d\n", getpid(), ++number);
        printf("父进程的父进程(终端进程), pid = %d\n", getppid());
        sleep(1);
    }
    else if(pid == 0)
    {
        // 子进程
        number += 100;
        printf("我是子进程, pid = %d, number = %d\n", getpid(), number);
        printf("子进程的父进程, pid = %d\n", getppid());
    }

    return 0;
}
