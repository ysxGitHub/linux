#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
// pthread_rwlock_t rwlock; // 读写锁
// pthread_rwlock_init();
// pthread_rwlock_destroy();

// 锁操作只能二选一, 不能 r\w 同时上锁

// pthread_rwlock_rdlock();
// pthread_rwlock_wrlock();

// pthread_rwlock_unlock()

// 读写锁的特点:
// 1. 使用读写锁的读锁锁定了临界区，线程对临界区的访问是并行的，读锁是共享的。
// 2. 使用读写锁的写锁锁定了临界区，线程对临界区的访问是串行的，写锁是独占的。
// 3. 使用读写锁分别对两个临界区加了读锁和写锁，两个线程要同时访问者两个临界区，
//    访问写锁临界区的线程继续运行，访问读锁临界区的线程阻塞，因为写锁比读锁的优先级高。

#define MAX 50

int num = 0;

pthread_rwlock_t rwlock;

void* writeNum(void* arg){
    for(int i=0; i<MAX; ++i){
        pthread_rwlock_wrlock(&rwlock);
        int cur = num;
        cur++;
        num = cur;
        printf("Thread wirte A id = %lu, num = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(rand()%3);
    }
    return NULL;
}

void* readNum(void* arg){
    for(int i=0; i<MAX; ++i){
        pthread_rwlock_rdlock(&rwlock);
        printf("Thread read B id = %lu, num = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(rand()%5);
    }
    return NULL;
}


int main(int argc, char const *argv[])
{
    pthread_t p1[5], p2[2];
    pthread_rwlock_init(&rwlock, NULL); 
    for(int i=0; i<5; i++){
        pthread_create(&p1[i], NULL, readNum, NULL);
    }
    for(int i=0; i<3; i++){
        pthread_create(&p2[i], NULL, writeNum, NULL);
    }
    for(int i=0; i<5; i++){
        pthread_join(p1[i], NULL);
    }
    for(int i=0; i<3; i++){
        pthread_join(p2[i], NULL);
    }
    pthread_rwlock_destroy(&rwlock);
    return 0;
}


