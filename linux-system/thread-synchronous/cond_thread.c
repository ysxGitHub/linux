#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep()

pthread_cond_t cond; // 要和 mutex 一起使用
pthread_mutex_t mutex;

struct Node{
    int num;
    struct Node* next;
};

// 头节点
struct Node* head = NULL;

void* producer(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node)); 
        newNode->num = rand()%100;
        newNode->next = head;
        head = newNode;
        printf("生产者, id: %ld, num: %d\n", pthread_self(), newNode->num);
        pthread_mutex_unlock(&mutex);
        // 唤醒被阻塞的消费者线程
        pthread_cond_broadcast(&cond);
        sleep(rand()%5);
    }
    return NULL;
}

void* consumer(void* arg){
    while (1){
        pthread_mutex_lock(&mutex);
        while(head==NULL){
            // 阻塞一个或多个消费者线程, 但会自动释放互斥锁
            pthread_cond_wait(&cond, &mutex);
            // 多个消费者线程被唤醒后会抢占 mutex
        }
        struct Node* node = head; 
        printf("消费者, id: %ld, num: %d\n", pthread_self(), node->num);
        head = head->next;
        free(node);
        pthread_mutex_unlock(&mutex);
        sleep(rand()%5);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t t1[5], t2[5];
    for(int i=0; i<5; i++){
        pthread_create(&t1[i], NULL, producer, NULL);
        pthread_create(&t2[i], NULL, consumer, NULL);
    }

    for(int i=0; i<5; i++){
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }   

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
