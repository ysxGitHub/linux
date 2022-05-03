#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

// pthread_mutex_trylock(); // 尝试申请锁，发现临界区被其他线程占用，则该线程去作其他事情
#define MAX 50

int num;

pthread_mutex_t mutex;

void* funcA_num(void* arg){
    for(int i=0; i<MAX; ++i){
        pthread_mutex_lock(&mutex);
        int cur = num;
        cur++;
        usleep(10);
        num = cur;
        printf("Thread A id = %lu, num = %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* funcB_num(void* arg){
    for(int i=0; i<MAX; ++i){
        pthread_mutex_lock(&mutex);
        int cur = num;
        cur++;
        num = cur;
        printf("Thread B id = %lu, num = %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex);
        usleep(10);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t p1, p2;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&p1, NULL, funcA_num, NULL);
    pthread_create(&p2, NULL, funcB_num, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    pthread_mutex_destroy(&mutex);
    
    return 0;
}

