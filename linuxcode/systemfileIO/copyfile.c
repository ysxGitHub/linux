#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/*
假设有一个比较大的磁盘文件，打开这个文件得到文件描述符 fd1，然后在创建一个新的磁盘文件得到文件描述符 fd2, 
    在程序中通过 fd1 将文件内容读出，并通过 fd2 将读出的数据写入到新文件中。
*/


int main(int argc, char const *argv[])
{
    // 1. 打开存在的文件123.txt，读这个文件
    int fd1 = open("./123.txt", O_RDONLY);
    if(fd1 == -1){
        perror("Open-readfile");
        return -1;
    }
    // 2. 打开不存在的文件, 将其创建出来, 将从english.txt读出的内容写入这个文件中
    int fd2 = open("./copy.txt", O_WRONLY|O_CREAT, 0664);
    if(fd2 == -1){
        perror("Open-writefile");
        return -1;
    }
    // 循环读文件
    char buf[4096];
    int len = -1;
    while((len=read(fd1, buf, sizeof(buf)))>0){
        // 将读到的数据写入到另一个文件中
        write(fd2, buf, len);
    }
    // 4. 关闭文件
    close(fd1);
    close(fd2);
    return 0;
}
