#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    // 打开文件
    int fd = open("123.txt", O_RDWR);
    if(fd==-1){
        printf("打开文件失败\n");
    }else{
        printf("fd: %d\n", fd);
    }
    close(fd);
    return 0;
}

