// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

typedef struct socketInfo{
    int fd;
    int epfd;
}SocketInfo;

void* acceptConn(void* arg);
void* communication(void* arg);

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

    // 现在只有监听的文件描述符
    // 所有的文件描述符对应读写缓冲区状态都是委托内核进行检测的epoll
    // 创建一个epoll模型
    int epfd = epoll_create(100);
    if(epfd == -1) {
        perror("epoll_create");
        exit(0);
    } 

    // 往epoll实例中添加需要检测的节点, 现在只有监听的文件描述符
    struct epoll_event ev;
    ev.events = EPOLLIN|EPOLLET;    // 检测lfd读读缓冲区是否有数据
    ev.data.fd = lfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
    if(ret == -1) {
        perror("epoll_ctl");
        exit(0);
    }
    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(struct epoll_event);
    // 持续检测
    while(1){
        // 调用一次, 检测一次
        int num = epoll_wait(epfd, evs, size, -1); // epoll线程安全函数不需要加锁
        printf("poll tree num: %d\n\n", num);
        pthread_t tid;
        for(int i=0; i<num; ++i) {
            SocketInfo* info = (SocketInfo*)malloc(sizeof(SocketInfo));
            // 取出当前的文件描述符
            info->fd = evs[i].data.fd;
            info->epfd = epfd;
            // 判断这个文件描述符是不是用于监听的
            if(info->fd == lfd) {
                pthread_create(&tid, NULL, acceptConn, info);
                pthread_detach(tid);
            } else{
                pthread_create(&tid, NULL, communication, info);
                pthread_detach(tid);               
            }           
        }
    }
    close(lfd);
    return 0;
}

void* acceptConn(void* arg) {
    SocketInfo* info = (SocketInfo*) arg;
    printf("accept thread: %ld\n\n", pthread_self());
    // 建立新的连接
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    int cfd = accept(info->fd, (struct sockaddr*)&cliaddr, &clilen);
    if(cfd == -1) {
        perror("accept");

        free(info);
        return NULL;
    }
    // 打印客户端的地址信息
    char ip[24] = {0};
    printf("客户端的IP地址: %s, 端口: %d\n",
        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
        ntohs(cliaddr.sin_port));                
    
    // 将文件描述符设置为非阻塞
    // 得到文件描述符的属性
    int flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);

    // 新得到的文件描述符添加到epoll模型中, 下一轮循环的时候就可以被检测了
    // 读缓冲区是否有数据, 并且将文件描述符设置为边沿模式
    struct epoll_event ev;
    ev.events = EPOLLIN|EPOLLET;    // 读缓冲区是否有数据
    ev.data.fd = cfd;
    int ret = epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);
    if(ret == -1) {
        perror("epoll_ctl-accept");
        
        free(info);
        return NULL;
    }
    
    free(info);
    return NULL;
}

void* communication(void* arg) {
    printf("communication thread: %ld\n\n", pthread_self());
    SocketInfo* info = (SocketInfo*) arg;
    
    // 接收数据
    char tmp[1024] = {0};
    char buf[5] = {0};
    while(1){
        int len = recv(info->fd, buf, sizeof(buf), 0);
        // printf("len = %d\n", len);
        if(len == 0) {
            printf("客户端关闭了连接...\n");
            // 将这个文件描述符从epoll模型中删除
            epoll_ctl(info->epfd, EPOLL_CTL_DEL, info->fd, NULL);
            close(info->fd);
            break;
        }
        else if(len > 0) {
            // 收到了数据
            printf("server recv buf: %s\n", buf);
            // 转换为大写
            for(int i=0; i<len; i++){
                buf[i] = toupper(buf[i]);
            }
            strncat(tmp+strlen(tmp), buf, len);
            // write(STDERR_FILENO, buf, len);
        }
        else {  // len = -1
            if(errno==EAGAIN){
                printf("数据读完了...\n");
                // 发送数据
                int ret = send(info->fd, tmp, sizeof(tmp), 0);
                if(ret == -1){
                    perror("write");
                }
                break;                            
            }else{
                // 异常
                perror("read");
                break; 
            }
        }
    }

    free(info);
    return NULL;
}