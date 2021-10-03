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
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    // 02. 初始化服务器 sockaddr_in 
    struct sockaddr_in server_addr;
    socklen_t serv_len = sizeof(server_addr);

    memset(&server_addr, 0, serv_len);
    server_addr.sin_family = AF_INET;                   // 地址族 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 监听本机所有的IP
    server_addr.sin_port = htons(PORT);                 // 设置端口 

    // 03. 绑定IP和端口
    bind(fd, (struct sockaddr*)&server_addr, serv_len);

    struct sockaddr_in client_addr;
    socklen_t cli_len = sizeof(client_addr);

    while(1)
    {
        // 04. 接收来自客户端的消息
        char buf[255] = {0};
        int ret = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &cli_len);
        if(ret == -1)
        {
            perror("recv error");
            exit(1);
        }
        else
        {
            // 05. 打印客户端信息
            char ip[64] = {0};
            printf("New Client IP: %s, Port: %d\n",
            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
            ntohs(client_addr.sin_port));

            printf("recv data: %s \n", buf);

            // 06. 给客户端发信息
            sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
        }
    }

    close(fd);
    return 0;
}
