#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

// 静态初始化变量
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 动态
pthread_cond_t cond;
pthread_mutex_t mutex;


void* thread_task(void* arg)
{
    // 无论哪种等待方式，都必须配合一个互斥锁使用，防止多个线程同时请求竞争条件。
    pthread_mutex_lock(&mutex);
    // 无条件等待条件成立
    pthread_cond_wait(&cond, &mutex);
    // 计时等待条件成立
    // pthread_cond_timedwait(&cond, &mutex, timespec*abstime);

    printf("thread_task[%u] start working!\n",(int)pthread_self());
    sleep(1);

    printf("thread_task[%u] work over!\n",(int)pthread_self());
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
    
}

void* broadcastDiffMutex(void* arg)
{
    // 唤醒所有正在pthread_cond_wait(&cond, &mutex)的线程
    pthread_cond_broadcast(&cond);
    printf("[%u] is waking up all pthreads!\n", (int)pthread_self());
    sleep(1);

    pthread_exit(0);
}

void* signalDiffMutex(void* arg)
{
    // 唤醒至少一个正在pthread_cond_wait(&cond, &mutex)的线程
    pthread_cond_signal(&cond);
    printf("[%u] is waking up someone pthreads!\n", (int)pthread_self());
    sleep(1);

    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_t thread_1, thread_2, thread_3;
    pthread_create(&thread_1, NULL, thread_task, NULL);
    pthread_create(&thread_2, NULL, thread_task, NULL);
    sleep(2);

    // 当使用signal方式时，thread_1和thread_2中只被唤醒了一个线程，
    // 没有唤醒的进程就处于pthread_cond_wait处
    // pthread_create(&thread_3, NULL, signalDiffMutex, NULL);
    // 当使用broadcast方式时，阻塞的进程都被唤醒了
    pthread_create(&thread_3, NULL, broadcastDiffMutex, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_3, NULL);

    pthread_cond_destroy(&cond);

    return 0;
}

