#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<strings.h>

#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

#define SHMSZ 128
#define PROJ_PATH "."
#define ID4SHM 1
#define ID4SEM 2


// 自定义的信号量操作联合体
union semun
{
    int val;
    struct semid_ds *buf;
    struct semifo *_buf;
};

// p操作
static void sem_p(int semid, int semnum)
{
    struct sembuf op[1];
    op[0].sem_num = semnum;
    op[0].sem_op = -1;
    op[0].sem_flg = 0;
    semop(semid, op, 1);
};

// v操作
static void sem_v(int semid, int semnum)
{
    struct sembuf op[1];
    op[0].sem_num = semnum;
    op[0].sem_op = 1;
    op[0].sem_flg = 0;
    semop(semid, op, 1);
};


// 初始化
static void sem_init(int semid, int semnum, int value)
{
    union semun a;
    a.val = value;
    semctl(semid, semnum, SETVAL, a);
};

int main(int argc, char const *argv[])
{
    // 获得SHM对应的key
    key_t shm_key = ftok(PROJ_PATH, ID4SHM);
    // 获得SEM对应的key
    key_t sem_key = ftok(PROJ_PATH, ID4SEM);
    // 获得SHM对应的ID， 并将它映射到本地进程
    int shm_id = shmget(shm_key, SHMSZ, IPC_CREAT|0644);
    char* shm_addr = (char *)shmat(shm_id, NULL, 0);

    // 获取SEM对应的ID， 若新建则初始化它，否则直接获取其ID 
    int sem_id = semget(sem_key, 2, IPC_CREAT|IPC_EXCL|0644);
    if((-1==sem_id)&&(EEXIST==errno))
    {
        sem_id = semget(sem_key, 2, 0644);
    }
    else
    {
        // 将第0个元素初始化为0，代表数据资源
        sem_init(sem_id, 0, 0);
        // 将第1个元素初始化为1，代表空间资源
        sem_init(sem_id, 1, 1);
    }
    while (1)
    {
        sem_p(sem_id, 0);
        printf("From Jack: %s\n",shm_addr);
        sem_v(sem_id, 1);
    }
    

    return 0;
}


