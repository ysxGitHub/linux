#include <stdio.h>
#define NUMBER 3

int func(int n){
    return n;
}

int main(){
    int a = 10;
#ifdef DEBUG
    printf("我是一个程序员，我也会爬树\n");
#endif
    for(int i=0; i<NUMBER; ++i){
        printf("%d: hello world!\n", func(i));
    }
    // hello world
    // printf("hello world!");
    return 0;
}
