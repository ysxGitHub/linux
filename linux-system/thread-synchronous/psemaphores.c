#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

sem_t semp, semc;
pthread_mutex_t mutex;

struct Node{
    int num;
    struct Node* next;
};

// 头节点
struct Node* head = NULL;

void* producer(void* arg){
    while(1){
        sem_wait(&semp);
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node)); 
        newNode->num = rand()%100;
        newNode->next = head;
        head = newNode;
        printf("生产者, id: %ld, num: %d\n", pthread_self(), newNode->num);
        pthread_mutex_unlock(&mutex);
        sem_post(&semc);
        sleep(rand()%5);
    }
    return NULL;
}

void* consumer(void* arg){
    while (1){
        // pthread_mutex_lock(&mutex); //不能放在这 会死锁，
        sem_wait(&semc);
        pthread_mutex_lock(&mutex);
        struct Node* node = head; 
        printf("消费者, id: %ld, num: %d\n", pthread_self(), node->num);
        head = head->next;
        free(node);
        pthread_mutex_unlock(&mutex);
        sem_post(&semp);
        sleep(rand()%5);
    }
    return NULL;
}


int main(int argc, char const *argv[])
{
    sem_init(&semp, 0, 5);
    sem_init(&semc, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t t1[5], t2[5];
    for(int i=0; i<5; i++){
        pthread_create(&t1[i], NULL, producer, NULL);
        pthread_create(&t2[i], NULL, consumer, NULL);
    }

    for(int i=0; i<5; i++){
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    } 

    sem_destroy(&semp);
    sem_destroy(&semc);

    return 0;
}


