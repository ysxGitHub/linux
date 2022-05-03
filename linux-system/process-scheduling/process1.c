#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t cpid;
    printf("父进程：%d\n",getpid());
    int value = 100;
    cpid = fork(); // 子进程 会复制父进程的 内存空间 

    // printf("cpid=%d\n",cpid);

    if(cpid==0) //子进程执行此代码
    {
        printf("子进程id(父进程id: %d): %d\n",getppid(),getpid());
        for(int i=0;i<3;i++)
        {
            printf("hello(%d)\n",value--);
            sleep(1);
        }
    }
    else  //父进程执行此代码
    {
        printf("父进程id: %d\n",getpid());
        for(int i=0;i<3;i++)
        {
            printf("world(%d)\n",value++);
            sleep(1);
        }
        wait(NULL); //等待子进程结束后父进程再结束

    }
    // 如果父进程提前结束子进程将变成孤儿进程，会托管给系统进程（pid=1）。

    return 0;
}