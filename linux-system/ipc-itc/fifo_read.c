#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
// #include<sys/types.h>
#include<sys/stat.h>

// 有名管道fifo的名字
#define FIFO "./fifotest"

int main(int argc, char const *argv[])
{
    // 判断文件是否存在，
    if(access(FIFO, F_OK))
    {
        // 不存在时进行创建
        mkfifo(FIFO, 0644);
    }    

    // 以只读的方式打开
    int fifo = open(FIFO, O_RDONLY);\
    // 计数，读取的次数
    int count = 0;
    char msg[200];

    while(1)
    {
        // 给buff清零
        bzero(msg, 200);
        // 从管道中读取数据
        read(fifo, msg, 200);
        printf("From FIFO read %d times, buffer: %s\n", ++count, msg);

        sleep(10);
    }


    return 0;
}


