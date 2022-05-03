#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define _GUN_SOURCE

#define handle_error_en(en, msg) \
    do                           \
    {                            \
        errno = en;              \
        perror(msg);             \
        exit(EXIT_FAILURE);      \
    } while (0)

static void display_pthread_attr(pthread_attr_t *attr, char *prefix)
{
    int s, i;
    size_t v;
    void *stkaddr;
    struct sched_param sp;

    s = pthread_attr_getdetachstate(attr, &i);
    if(s!=0)
        handle_error_en(s, "pthread_attr_getdetachstate");
    printf("%sDetach state        = %s\n", prefix,
        (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED":
        (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE":
        "???");

    s = pthread_attr_getscope(attr, &i);
    if(s!=0)
        handle_error_en(s, "pthread_attr_getscope");
    printf("%sScope               = %s\n", prefix,
        (i == PTHREAD_SCOPE_SYSTEM) ? "PTHREAD_SCOPE_SYSTEM":
        (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS":
        "???");

    s = pthread_attr_getinheritsched(attr, &i);
    if(s!=0)
        handle_error_en(s, "pthread_attr_getinheritsched");
    printf("%sInherit scheduler   = %s\n", prefix,
        (i == PTHREAD_INHERIT_SCHED) ? "PTHREAD_INHERIT_SCHED":
        (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED":
        "???");

    s = pthread_attr_getschedpolicy(attr, &i);
    if(s!=0)
        handle_error_en(s, "pthread_attr_getschedpolicy");
    printf("%sScheduling policy   = %s\n", prefix,
        (i == SCHED_OTHER) ? "SCHED_OTHER":
        (i == SCHED_FIFO) ? "SCHED_FIFO":
        (i == SCHED_RR) ? "SCHED_RR":
        "???");
    
    s = pthread_attr_getschedparam(attr, &sp);
    if (s!=0)
        handle_error_en(s, "pthread_attr_getschedparam");
    printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);

    s = pthread_attr_getguardsize(attr, &v);
    if (s!=0)
        handle_error_en(s, "pthread_attr_getguardsize");
    printf("%sGuard size          = %zu bytes\n", prefix, v);

    s = pthread_attr_getstack(attr, &stkaddr, &v);
    if (s!=0)
        handle_error_en(s, "pthread_attr_getstack");
    printf("%sStack address       = %p\n", prefix, stkaddr);
    printf("%sStack size          = 0x%zx bytes\n", prefix, v);   

}

void* computing(void* arg)
{
    int s;
    pthread_attr_t gattr;
    // 得到该线程的属性值
    s = pthread_getattr_np(pthread_self(), &gattr);
    if(s!=0)
        handle_error_en(s, "pthread_getattr_np");
    
    printf("Thread attributes:\n");
    display_pthread_attr(&gattr, "\t");
    // 暂停
    pause();
    // 线程返回值
    pthread_exit(10);
}

int main(int argc, char const *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    // 初始化线程属性 DEFAULT
    pthread_attr_init(&attr);

    /*
    线程创建函数
    pthread_create()的参数
        1.线程id的地址
        2.线程属性的地址
        3.线程函数的地址
        4.线程参数的地址
    */  

    pthread_create(&tid, &attr, computing, NULL);

    // 等待指定的线程结束,
    pthread_join(tid, NULL);

    return 0;
}

/*
Thread attributes:
	Detach state        = PTHREAD_CREATE_JOINABLE   // 可等待 pthread_join
	Scope               = PTHREAD_SCOPE_SYSTEM      // system / process (线程竞争的范围)
	Inherit scheduler   = PTHREAD_INHERIT_SCHED     // 
	Scheduling policy   = SCHED_OTHER               // 调度策略
	Scheduling priority = 0                         // 调度优先级
	Guard size          = 4096 bytes                // 
	Stack address       = 0x7fcc7f885000            //
	Stack size          = 0x800000 bytes            //


0 S  1000    4949    4899  0  80   0 - 19095 futex_ pts/1    00:00:00 LinuxTread
              PID    PPID  
0 S ysx      4949    4899  0  80   0 - 19095 futex_   612   1 16:55 pts/1    00:00:00 ./LinuxTread
0 S ysx      5054    4874  0  80   0 -  3000 pipe_r   724   0 16:58 pts/0    00:00:00 grep --color=auto Linux
*/