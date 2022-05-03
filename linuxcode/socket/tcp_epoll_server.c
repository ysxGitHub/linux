// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

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
    ev.events = EPOLLIN;    // 检测lfd读读缓冲区是否有数据
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
        int num = epoll_wait(epfd, evs, size, -1);
        printf("poll tree num: %d\n\n", num);
        for(int i=0; i<num; ++i) {
            // 取出当前的文件描述符
            int curfd = evs[i].data.fd;
            // 判断这个文件描述符是不是用于监听的
            if(curfd == lfd) {
                // 建立新的连接
                struct sockaddr_in cliaddr;
                int clilen = sizeof(cliaddr);
                int cfd = accept(curfd, (struct sockaddr*)&cliaddr, &clilen);
                if(cfd == -1) {
                    perror("accept");
                    exit(0);
                }
                // 打印客户端的地址信息
                char ip[24] = {0};
                printf("客户端的IP地址: %s, 端口: %d\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
                    ntohs(cliaddr.sin_port));                

                // 新得到的文件描述符添加到epoll模型中, 下一轮循环的时候就可以被检测了
                ev.events = EPOLLIN;    // 读缓冲区是否有数据
                ev.data.fd = cfd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                if(ret == -1) {
                    perror("epoll_ctl-accept");
                    exit(0);
                }
            } else{
                // 接收数据
                char buf[1024] = {0};
                // 如果一次只能接收10个字节, 客户端一次发送100个字节
                // 一次是接收不完的, 文件描述符对应的读缓冲区中还有数据
                // 下一轮select检测的时候, 内核还会标记这个文件描述符缓冲区有数据 -> 再读一次
                // 	循环会一直持续, 知道缓冲区数据被读完位置
                int len = read(curfd, buf, sizeof(buf));
                if(len == 0) {
                    printf("客户端关闭了连接...\n");
                    // 将这个文件描述符从epoll模型中删除
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                }
                else if(len > 0) {
                    // 收到了数据
                    printf("server recv buf: %s\n", buf);
                    // 转换为大写
                    for(int i=0; i<len; i++){
                        buf[i] = toupper(buf[i]);
                    }
                    // 发送数据
                    ret = write(curfd, buf, strlen(buf)+1);
                    if(ret == -1){
                        perror("write");
                        exit(0); 
                    }
                }
                else {
                    // 异常
                    perror("read");
                    exit(0); 
                }
            }           
        }
    }
    close(lfd);
    return 0;
}

