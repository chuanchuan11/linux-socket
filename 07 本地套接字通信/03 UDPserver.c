#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define DES_PATH "udpserver.socket"

int main()
{
    //01. 创建socket
    int fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        perror("creat socket error");
        exit(-1);
    }
    
    //02. 如果文件存在，则先删除，能够快速重入
    unlink(DES_PATH);

    //03. 服务器端绑定套接字文件
    struct sockaddr_un server;
    memset(&server, 0, sizeof(server));
    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, DES_PATH, sizeof(server.sun_path) - 1);
    int bret = bind(fd, (struct sockaddr *)&server, sizeof(server));
    if(bret < 0)
    {
        perror("bind error");
        exit(-1);
    }
    struct sockaddr_un client;
    socklen_t cli_len = sizeof(client);
    while(1)
    {
        char buf[255] = {0};
        
        //04. 等待接收客户端的信息
        int ret = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client, &cli_len);        
        if(ret == -1)
        {
            printf("recv error \n");
        } 
        else
        {
            printf("Recvfrom from %s ==> %d bytes, data:%s \n", client.sun_path, ret, buf);
            sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&client, sizeof(client));
        }    
    }

    close(fd);
    return 0;
}
