#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<pthread.h>

struct Test{
    int num;
    int age;
};

// struct Test t; // 方法1：全局变量可以

// 线程函数
void* callback(void* arg)
{
    struct Test* t = (struct Test*) arg;

    printf("在一个新的进程中: %lu\n", pthread_self());
    // struct Test t; // 不能放在栈区 会被释放
    t->num = 100;
    t->age = 66; 

    pthread_exit(t); 

    return NULL;
    // return &t;
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

    struct Test t; // 方法2：使用主线程的栈空间, 传入回调函数

    pthread_create(&tid, NULL, callback, &t);

    void * ptr;

    // 阻塞函数, 等待指定的线程结束, 线程回收, 不接受子线程传出的数据
    // pthread_join(tid, NULL);
    // 阻塞函数, 等待指定的线程结束, 线程回收, 接受子线程传出的数据 
    pthread_join(tid, &ptr);

    // struct Test* pt = (struct Test*) ptr;
    printf("num = %d, age = %d\n", t.num, t.age);

    printf("在主线程中\n");

    return 0;
}

// run:  gcc helloThread.c -o helloThread -pthread
