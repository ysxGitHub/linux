// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

// 信息结构体
struct SockInfo{
    struct sockaddr_in addr;
    int fd;
};

struct SockInfo infos[512];
void* working(void* arg);

int main()
{
    // 1. 创建监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1) {
        perror("socket");
        exit(0);
    }

    // 2. 将socket()返回值和本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10001);   // 大端端口
    // INADDR_ANY代表本机的所有IP, 假设有三个网卡就有三个IP地址
    // 这个宏可以代表任意一个IP地址
    // 这个宏一般用于本地的绑定操作
    addr.sin_addr.s_addr = INADDR_ANY;  // 这个宏的值为0 == 0.0.0.0
    //    inet_pton(AF_INET, "192.168.237.131", &addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1) {
        perror("bind");
        exit(0);
    }

    // 3. 设置监听
    ret = listen(lfd, 128);
    if(ret == -1) {
        perror("listen");
        exit(0);
    }

    //初始化结构体数组
    int max = sizeof(infos)/sizeof(infos[0]);
    for(int i=0; i<max; i++){
        bzero(&infos[i], sizeof(infos[i]));
        infos[i].fd = -1;
    }

    // 4. 阻塞等待并接受客户端连接
    // struct sockaddr_in cliaddr;
    int clilen = sizeof(struct sockaddr_in);
    while(1){
        struct SockInfo* pinfo;
        for(int i=0; i<max; i++){
            if(infos[i].fd==-1){
                pinfo = & infos[i];
                break;
            }
        }
        int cfd = accept(lfd, (struct sockaddr*)&pinfo->addr, &clilen);
        if(cfd == -1) {
            perror("accept");
            break;
        }
        pinfo->fd = cfd;
        // 创建子线程
        pthread_t tid;
        pthread_create(&tid, NULL, working, pinfo);
        pthread_detach(tid);
    }
    close(lfd);
    return 0;
}
void* working(void* arg){
    printf("communication thread: %ld\n\n", pthread_self());

    struct SockInfo* info= (struct SockInfo*)arg;
    // 打印客户端的地址信息
    char ip[24] = {0};
    printf("客户端的IP地址: %s, 端口: %d\n",
           inet_ntop(AF_INET, &info->addr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(info->addr.sin_port));

    // 5. 和客户端通信
    while(1) {
        // 接收数据
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = read(info->fd, buf, sizeof(buf));
        if(len > 0) {
            printf("客户端say: %s\n", buf);
            write(info->fd, buf, len);
        }
        else if(len  == 0) {
            printf("客户端断开了连接...\n");
            break;
        }
        else {
            perror("read");
            break;
        }
    }

    close(info->fd);
    info->fd = -1;
    // free(info);
    return NULL;
}

