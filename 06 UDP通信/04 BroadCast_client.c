#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>


int main()
{
    // 01. 创建套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    // 02. 绑定要接收的广播IP和端口
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(6767);                            //接收广播的端口
    inet_pton(AF_INET, "0.0.0.0", &client.sin_addr.s_addr);   // 接收广播的IP，0.0.0.0为接收所有广播IP
    bind(fd, (struct sockaddr*)&client, sizeof(client));

    while(1)
    {
        // 03. 接受广播消息
        char buf[255] = {0};
        
        int ret = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        if(ret == -1)
        {
            perror("send error");
            exit(1);
        }
        else
        {
            // 04. 接收来自广播的信息
            char ip[64] = {0};
            printf("Recv BroadCast Data: %s \n", buf);
        }
    }

    close(fd);
    return 0;
}
