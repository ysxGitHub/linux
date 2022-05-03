#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NUM 10

int main(int argc, char* argv[])
{
    printf("Hello world\n");
    printf("参数的个数：%d\n", argc);
    for(int i=0; i<argc; ++i){
        printf("%d\n", NUM);
        printf("参数 %d: %s\n", i, argv[i]);
    }
    return 0;
}

