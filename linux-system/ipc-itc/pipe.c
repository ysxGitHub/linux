#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<error.h>
// #include<pthread.h>




int main(int argc, char const *argv[])
{
    //用于存放PIPE的两个文件描述符
    int fd[2];
    // 创建PIPE，并将文件描述符存放在fd数组中
    if(-1 == pipe(fd))
    {
        perror("pipe()");
        exit(1);
    }

    // 创建一个子进程，它将会继承PIPE的描述符，fork()的返回值：子进程和父进程的pid不同
    pid_t pid = fork();
    if(pid<0)
    {
        perror("fork()");
        exit(1);
    }

    // 子进程返回时的 pid==0
    if(0 == pid)
    {
        // 关闭读端
        close(fd[0]);
        char *s = "Hello World!\n";
        // 通过写端fd[1]将数据写入到PIPE中
        printf("write buffer to father: %s\n", s);
        write(fd[1], s, strlen(s));
    }

    // 父进程返回时pid>0，此时pid为子进程的进程号
    if(pid>0)
    {
        // 关闭写端
        close(fd[1]);
        char buff[30];
        // 给buff清零
        bzero(buff, 30);
        // memset(buff,0,30);
        // 通过读端fd[0]将数据从PIPE中读出来
        read(fd[0], buff, 30);
        printf("read buffer from child: %s\n", buff);
    }

    close(fd[0]);
    close(fd[1]);

    return 0;
}
