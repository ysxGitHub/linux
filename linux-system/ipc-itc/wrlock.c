#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

pthread_rwlock_t rwlock;
int global_num = 10;

void* thread_read_task(void* arg)
{
    while (1)
    {
        // 加读锁 
        pthread_rwlock_rdlock(&rwlock);
        printf("Read: thread[%d] entering the critical area!\n", (int)pthread_self());
        printf("Read: global_num = %d\n", global_num);
        sleep(1);
        printf("Read: thread[%d] exiting the critical area!\n", (int)pthread_self());
        // 解读锁
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
        
    }
    // pthread_exit(0);
}

void* thread_write_task(void* arg)
{
    while (1)
    {
        // 加读锁 
        pthread_rwlock_wrlock(&rwlock);
        printf("Write: thread[%d] entering the critical area!\n", (int)pthread_self());
        global_num++;
        printf("Write: global_num = %d\n", global_num);
        sleep(1);
        printf("Write: thread[%d] exiting the critical area!\n", (int)pthread_self());
        // 解读锁
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
        
    }
    // pthread_exit(0);
}




int main(int argc, char const *argv[])
{
    pthread_rwlock_init(&rwlock, NULL);

    pthread_t tid_read1, tid_read2, tid_write1, tid_write2;
    
    pthread_create(&tid_read1,NULL,thread_read_task,NULL);
    pthread_create(&tid_read2,NULL,thread_read_task,NULL);
    pthread_create(&tid_write1,NULL,thread_write_task,NULL);
    pthread_create(&tid_write2,NULL,thread_write_task,NULL);

    pthread_join(tid_read1,NULL);
    pthread_join(tid_read2,NULL);
    pthread_join(tid_write1,NULL);
    pthread_join(tid_write2,NULL);

    return 0;
}

