#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    // 创建新文件之前, 先检测是否存在
    // 文件存在创建失败, 返回-1, 文件不存在创建成功, 返回分配的文件描述符
    int fd = open("./new.txt", O_CREAT|O_EXCL|O_RDWR);
    if(fd==-1){
        printf("创建文件失败， 已经存在了，fd: %d\n", fd);
    }else{
        printf("创建文件成功， fd: %d\n", fd);
    }
    close(fd);
    return 0;
}




