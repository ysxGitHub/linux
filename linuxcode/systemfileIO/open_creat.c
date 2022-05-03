#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main()
{
     // 创建文件
     int fd = open("./new.txt", O_CREAT|O_RDWR);
     if(fd==-1){
          printf("打开文件失败\n");
     }else{
          printf("创建文件成功, fd: %d\n", fd);
     }
     close(fd);
     return 0;
}

