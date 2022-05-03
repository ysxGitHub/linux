#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
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
    // 以只写的方式打开
    int fifo = open(FIFO, O_WRONLY);

    char *msg = "Hello, I ma writing...";
    // 统计写的次数
    int count = 0;

    while (1)
    {
        // 将数据写入fifo
        int n = write(fifo, msg, strlen(msg));
        printf("Writed %d times, %d bytes have been sended.\n",++count, n);
        sleep(1);
    }
    

    return 0;
}
