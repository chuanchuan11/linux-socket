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
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    //02. 如果套接字文件存在, 删除套接字文件，实现快速重入
    unlink("TCPserver.socket");

    //03. 服务器端绑定到套接字文件
    struct sockaddr_un server;
    server.sun_family = AF_LOCAL;
    strcpy(server.sun_path, "TCPserver.socket"); //最好使用绝对路径，防止客户端和服务端路径不一致出错
    bind(lfd, (struct sockaddr*)&server, sizeof(server));

    //04. 监听
    listen(lfd, 36);

    //05. 等待连接请求
    struct sockaddr_un client;
    socklen_t len = sizeof(client);
    int cfd = accept(lfd, (struct sockaddr*)&client, &len);
    if(cfd == -1)
    {
        perror("accept error");
        exit(1);
    }
    printf("client bind file ===> %s\n", client.sun_path);

    //06. 通信
    while(1)
    {
        char buf[1024] = {0};
        int recvlen = recv(cfd, buf, sizeof(buf), 0);
        if(recvlen == -1)
        {
            perror("recv error");
            exit(1);
        }
        else if(recvlen == 0)
        {
            printf("clietn disconnect ....\n");
            close(cfd);
            break;
        }
        else
        {
            printf("server recv data: %s\n", buf);
            send(cfd, buf, recvlen, 0);
        }
    }
    close(cfd);
    close(lfd);
    return 0;
}
