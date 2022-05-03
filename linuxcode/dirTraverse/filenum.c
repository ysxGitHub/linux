	// filenum.c
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <dirent.h>
	
	int main(int argc, char* argv[])
	{
	    // 1. 打开目录
	    DIR* dir = opendir(argv[1]);
	    if(dir == NULL)
	    {
	        perror("opendir");
	        return -1;
	    }
	
	    // 2. 遍历当前目录中的文件
	    int count = 0;
	    while(1)
	    {
	        struct dirent* ptr = readdir(dir);
	        if(ptr == NULL)
	        {
	            printf("目录读完了...\n");
	            break;
	        }
	        // 读到了一个文件
	        // 判断文件类型
	        if(ptr->d_type == DT_REG)
	        {
	            char* p = strstr(ptr->d_name, ".mp3");
	            if(p != NULL && *(p+4) == '\0')
	            {
	                count++;
	                printf("file %d: %s\n", count, ptr->d_name);
	            }
	        }
	    }
	
	    printf("%s目录中mp3文件的个数: %d\n", argv[1], count);
	
	    // 关闭目录
	    closedir(dir);
	
	    return 0;
}