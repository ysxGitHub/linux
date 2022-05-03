#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>


void* calculate_pi(void* arg)
{
    unsigned int seed = time(NULL);
    int circle_points = 0;
    int square_points = 0;

    int intervals = *((int*)arg);

    for (int i=0; i<intervals*intervals; i++){

        double rand_x = (double)rand_r(&seed)/RAND_MAX;
        double rand_y = (double)rand_r(&seed)/RAND_MAX;

        if ((rand_x*rand_x)+(rand_y*rand_y)<=1){
            circle_points++;
        }
        square_points++;

    }
    double pi = (double)(4.0*circle_points)/square_points;
    printf("The estimated PI is %lf in %d points\n",pi, intervals*intervals);
    // 返回线程，表示线程结束  便于主线程判断子线程的结束状态
    pthread_exit(0);
}


int main(int argc, char const *argv[])
{
    clock_t start, delta;
    double time_used;

    start = clock();

    pthread_t calculate_pi_threads[10];
    int args[10];

    for(int i=0; i<10; i++){
        args[i] = 1000*(i+1);
        pthread_create(calculate_pi_threads+i, NULL, calculate_pi, args+i);
    }

    for(int i=0; i<10; i++){
        pthread_join(calculate_pi_threads[i],NULL);
    }

    delta = clock()-start;
    printf("The time taken in total: %lf seconds\n", (double)delta/CLOCKS_PER_SEC);
    return 0;
}
