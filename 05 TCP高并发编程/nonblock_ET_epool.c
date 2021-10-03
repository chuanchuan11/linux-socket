#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8888

int main()
{
    // 01. 创建套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    // 02. 初始化服务器 sockaddr_in 
    struct sockaddr_in server_addr;
    socklen_t serv_len = sizeof(server_addr);

    memset(&server_addr, 0, serv_len);
    server_addr.sin_family = AF_INET;                   // 地址族 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 监听本机所有的IP
    server_addr.sin_port = htons(PORT);                 // 设置端口 

    // 03. 绑定IP和端口
    bind(lfd, (struct sockaddr*)&server_addr, serv_len);

    // 04. 设置同时监听的最大个数
    listen(lfd, 36);
    printf("Start accept ......\n");

    struct sockaddr_in client_addr;
    socklen_t cli_len = sizeof(client_addr);

    // 05. 创建epoll树根节点
    int epfd = epoll_create(2000);

    // 06. 设置需要监听的事件
    struct epoll_event event;
    event.events = EPOLLIN;  
    event.data.fd = lfd;

    // 07. 添加到epoll节点上
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &event);

    // 08. 定义要返回的事件
    struct epoll_event all[2000];
    while(1)
    {
        // 09. 委托内核做IO检测, 阻塞等待
        int ret = epoll_wait(epfd, all, sizeof(all)/sizeof(all[0]), -1);
        
        // 10. 遍历all数组中前ret个元素，有事件发生的所有fd
        for(int i=0; i<ret; i++)
        {
            int fd = all[i].data.fd;
            
            // 11. 判断是否有新的连接
            if(fd == lfd)
            {
                // 12. 接受连接请求
                int cfd = accept(lfd, (struct sockaddr*)&client_addr, &cli_len);
                if(cfd == -1)
                {
                    perror("accept error");
                    exit(1);
                }
                else
                {
                    // 设置文件cfd为非阻塞模式
                    int flag = fcntl(cfd, F_GETFL);
                    flag |= O_NONBLOCK;
                    fcntl(cfd, F_SETFL, flag);

                    // 13. 将新的cfd添加到epoll节点上
                    struct epoll_event temp;
                    temp.events = EPOLLIN | EPOLLET;
                    temp.data.fd = cfd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &temp);

                    // 14. 打印客户端信息
                    char ip[64] = {0};
                    printf("New Client IP: %s, Port: %d\n",
                    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
                    ntohs(client_addr.sin_port));
                }
            }
            else
            {
                // 15. 处理已经连接的客户端发送过来的数据
                if(!all[i].events & EPOLLIN) 
                {
                    continue; //没有读事件
                }

                // 16. 读数据
                char buf[6] = {0};
                int len;
                // 循环读数据
                while( (len = recv(fd, buf, sizeof(buf)-1, 0)) > 0 )
                {
                    // 数据打印到终端
                    write(STDOUT_FILENO, buf, len);
                    // 发送给客户端
                    send(fd, buf, len, 0);
                }
                if(len == 0)
                {
                    printf("客户端断开了连接\n");
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    if(ret == -1)
                    {
                        perror("epoll_ctl - del error");
                        exit(1);
                    }
                    close(fd);
                }
                else if(len == -1)
                {
                    if(errno == EAGAIN)
                    {
                        printf("缓冲区数据已经读完\n");
                    }
                    else
                    {
                        printf("recv error----\n");
                        exit(1);
                    }
                }
            }
        }
    }

    close(lfd);
    return 0;
}
