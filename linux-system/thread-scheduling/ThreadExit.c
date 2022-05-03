#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<pthread.h>

// 线程函数
void* callback(void* arg)
{
    int num = *(int*) arg;
    for(int i=0; i<5; i++){
        printf("在一个新的进程中, num = %d\n", num);
    }
    return NULL;
}


int main(int argc, char const *argv[])
{
    // 线程id
    pthread_t tid;
    /*
    线程创建函数
    pthread_create()的参数
        1.线程id的地址
        2.线程属性的地址
        3.线程函数的地址
        4.线程参数的地址
    */
    int b = 2;
    pthread_create(&tid, NULL, callback, (void*)&b);

    // 等待指定的线程结束
    // pthread_join(tid, NULL);

    printf("在主线程中: %lu\n", pthread_self());

    // 主线程退出，不影响子线程
    pthread_exit(NULL);
    
    return 0;
}

// run:  gcc helloThread.c -o helloThread -pthread