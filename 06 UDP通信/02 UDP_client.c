#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8888
#define IP "127.0.0.1"

int main()
{
    // 01. 创建套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    // 02. 初始化服务器 sockaddr_in 
    struct sockaddr_in server_addr;
    socklen_t serv_len = sizeof(server_addr);

    memset(&server_addr, 0, serv_len);
    server_addr.sin_family = AF_INET;                     // 地址族          
    inet_pton(AF_INET, IP, &server_addr.sin_addr.s_addr); // 连接的IP
    server_addr.sin_port = htons(PORT);                   // 设置端口 
    while(1)
    {
        // 03. 给客户端发送消息
        char buf[255] = {0};

        fgets(buf, sizeof(buf), stdin);
        
        int ret = sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&server_addr, serv_len);
        if(ret == -1)
        {
            perror("send error");
            exit(1);
        }
        else
        {
            // 06. 接收来自给客户端发信息
            recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
            printf("recv data: %s \n", buf);
        }
    }

    close(fd);
    return 0;
}
