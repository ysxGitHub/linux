#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
// 如何创建子进程

int main(int argc, char const *argv[])
{
    // pid_t 是数据类型，实际上是一个整型
    // 通过typedef 重新定义一个名称，用于存储进程id
    pid_t ppid; //父进程id
    pid_t cpid; //子进程id

    // getpid()获得当前进程的id号
    // 使用ps -al命令 获得linux后台进程
    printf("主进程的id: %d\n", getpid());
    /*
    fork()函数用于创建一个新的进程，该进程为当前进程的子进程，
        子进程是父进程的复印份，会造成并发执行，
        也就是fork()后面的代码子进程和父进程都会执行。
    fork()的返回值：
        如果成功创建子进程，对于父子进程fork会返回不同的值，
        对于父进程它的返回值是子进程的进程id，
        对于子进程它的返回值是0，
        创建失败返回-1
    */

    cpid = fork();
    // printf("子进程的id: %d\n", cpid);
    
    // 所以此处会执行两次 id 一个父进程，另一个是子进程
    printf("子进程的id: %d\n", getpid());
    pause();
    return 0;}
