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

    // 02. 指定广播的IP和端口
    struct sockaddr_in addr_to;
    memset(&addr_to, 0, sizeof(addr_to));
    addr_to.sin_family = AF_INET;
    addr_to.sin_port = htons(6767);                                  // 配置广播使用的端口号
    inet_pton(AF_INET, "10.93.9.255", &addr_to.sin_addr.s_addr);     // 配置直接广播地址
                                                                     // 255.255.255.255有限广播地址
    // 03. 使能广播权限
    int flag = 1;
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));

    while(1)
    {
        // 04. 从广播地址发送广播消息
        char buf[255] = {0};
        fgets(buf, sizeof(buf), stdin);
        int ret = sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr *)&addr_to, sizeof(addr_to));
        if(ret == -1)
        {
            perror("sednto client error");
            exit(1);
        }
        else
        {
            printf("send broadcase messages: %s \n", buf);
        }
    }

    close(fd);
    return 0;
}
