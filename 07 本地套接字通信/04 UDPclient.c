#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <time.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>

int main()
{
    //01. 创建socket
    int fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        perror("creat socket error");
        exit(-1);
    }

    unlink("udpclient.socket");

    //02. 客户端绑定到套接字文件
    struct sockaddr_un client;
    memset(&client, 0, sizeof(client));
    client.sun_family = AF_LOCAL;
    strcpy(client.sun_path, "udpclient.socket");
    int bret = bind(fd, (struct sockaddr *)&client, sizeof(client)); 
    if(bret < 0)
    {
        perror("bind error");
        exit(-1);
    }

    //03. 初始化服务器端信息
    struct sockaddr_un server;
    socklen_t len = sizeof(server);
    memset(&server, 0, sizeof(server));
    server.sun_family = AF_LOCAL;
    strcpy(server.sun_path, "udpserver.socket");

    while(1)
    {
        char buf[255] = {0};

        fgets(buf, sizeof(buf), stdin);
        //04. 接收发送数据
        sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));

        int recv_len = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        if(recv_len == -1)
        {
            printf("recv error \n");
        }
        else
        {
            printf("client recv %d bytes, data: %s \n", recv_len, buf);
        }
    }

    close(fd);
    return 0;
}


