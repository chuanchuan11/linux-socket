#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

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

    // 05. 文件描述符集集合处理
    int maxfd = lfd;        // 最大的文件描述符
    fd_set reads, temp;     // 文件描述符读集合
    FD_ZERO(&reads);        // init
    FD_SET(lfd, &reads);    // add fd to set

    while(1)
    {
        // 06. 委托内核做IO检测
        temp = reads;
        int ret = select(maxfd+1, &temp, NULL, NULL, NULL);
        if(ret == -1)
        {
            perror("select error");
            exit(1);
        }
        // 07. 客户端发起了新的连接
        if(FD_ISSET(lfd, &temp))
        {
            // 08. 接受连接请求 - accept不阻塞
            int cfd = accept(lfd, (struct sockaddr*)&client_addr, &cli_len);
            if(cfd == -1)
            {
                perror("accept error");
                exit(1);
            }
            char ip[64];
            printf("new client IP: %s, Port: %d\n", 
                   inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
                   ntohs(client_addr.sin_port));
            // 09. 将cfd加入到待检测的读集合中 - 下一次就可以检测到了
            FD_SET(cfd, &reads);
            // 10. 更新最大的文件描述符
            maxfd = maxfd < cfd ? cfd : maxfd;
        }
        else
        {
            // 11. 已经连接的客户端有数据到达
            for(int i=lfd+1; i<=maxfd; ++i)
            {
                if(FD_ISSET(i, &temp))
                {
                    char buf[1024] = {0};
                    int len = recv(i, buf, sizeof(buf), 0);
                    if(len == -1)
                    {
                        perror("recv error");
                        exit(1);
                    }
                    else if(len == 0)
                    {
                        printf("客户端已经断开了连接\n");
                        close(i);
                        // 12. 从读集合中删除
                        FD_CLR(i, &reads);
                    }
                    else
                    {
                        printf("recv buf: %s\n", buf);
                        send(i, buf, strlen(buf)+1, 0);
                    }
                }
            }
        }

    }

    close(lfd);
    return 0;
}

