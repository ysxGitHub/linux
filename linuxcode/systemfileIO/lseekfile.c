#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("./hello.txt", O_RDWR);
    if(fd==-1){
        perror("open");
        return -1;
    }
    // 文件拓展，一共增加了1001个字节
    lseek(fd, 1000, SEEK_END);
    write(fd, " ", 1);

    close(fd);
    return 0;
}

