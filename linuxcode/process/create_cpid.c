/*
掌握了进程创建函数之后，实现一个简单的功能，在一个父进程中循环创建 3 个子进程，
    也就是最后需要得到 4 个进程，1 个父进程，3 个子进程，为了方便验证程序的正确性，
    要求在程序中打印出每个进程的进程 ID。
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main()
{
    
    pid_t pid;
    
    for(int i=0; i<3; i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    }
    printf("当前进程pid: %d\n", getpid());

    return 0;
}

