#include <stdio.h>
#include <sys/stat.h>

int main()
{
    // 1.定义结构体，存储文件信息
    struct stat myst;
    // 2.获取文件属性 123.txt
    int ret = stat("./123.txt", &myst);
    if(ret==-1){
        perror("stat");
        return -1;
    }
    printf("文件大小：%d\n", (int)myst.st_size);
    
    //判断文件类型
    if(S_ISREG(myst.st_mode)){
        printf("这是一个普通文件....\n");
    }
    if(S_ISDIR(myst.st_mode)){
        printf("这是一个文件夹....\n");
    }
    if(S_ISLNK(myst.st_mode)){
        printf("这是一个软连接文件....\n");
    }
    // 文件所有者对文件的操作权限
    printf("文件所有者对文件的操作权限: ");
    if(myst.st_mode & S_IRUSR)
    {
        printf("r");
    }
    if(myst.st_mode & S_IWUSR)
    {
        printf("w");
    }
    if(myst.st_mode & S_IXUSR)
    {
        printf("x");
    }
    printf("\n");

    return 0;
}

