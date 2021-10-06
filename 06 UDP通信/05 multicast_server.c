#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

int main()
{
    // 01. 创建套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    // 02. 指定组播的IP和端口
    struct sockaddr_in addr_to;
    memset(&addr_to, 0, sizeof(addr_to));
    addr_to.sin_family = AF_INET;
    addr_to.sin_port = htons(6767);                                  // 配置组播使用的端口号
    inet_pton(AF_INET, "239.0.0.10", &addr_to.sin_addr.s_addr);      // 配置组播地址
                                                                     // 255.255.255.255有限广播地址
    // 03. 使能组播权限并初始化
    struct ip_mreqn right;
    // init right
    inet_pton(AF_INET, "239.0.0.10", &right.imr_multiaddr.s_addr);   // 组播地址
    inet_pton(AF_INET, "0.0.0.0", &right.imr_address.s_addr);        // 本地IP
    right.imr_ifindex = if_nametoindex("enp9s0f2");
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &right, sizeof(right));

    while(1)
    {
        // 04. 发送组播消息
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
