#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main()
{
    //01. 创建socket
    int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket error");
        exit(1);
    }

    //02. 如果套接字文件存在, 删除套接字文件，实现快速重入
    //unlink("TCPclient.socket");

    //03. 客户端绑定到套接字文件
    struct sockaddr_un client;
    client.sun_family = AF_LOCAL;
    strcpy(client.sun_path, "TCPclient.socket");
    bind(fd, (struct sockaddr*)&client, sizeof(client));

    //04. 初始化server信息
    struct sockaddr_un serv;
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "TCPserver.socket");

    //05. 连接服务器
    connect(fd, (struct sockaddr*)&serv, sizeof(serv));

    //06. 通信
    while(1)
    {
        char buf[1024] = {0};
        fgets(buf, sizeof(buf), stdin);
        send(fd, buf, strlen(buf)+1, 0);

        // 接收数据
        recv(fd, buf, sizeof(buf), 0);
        printf("client recv data: %s\n", buf);
    }

    close(fd);
}
