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

    // 02. 绑定要接收的组播IP和端口
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(6767);                             //接收组播的端口
    inet_pton(AF_INET, "0.0.0.0", &client.sin_addr.s_addr);   // 0.0.0.0为接收所有组播IP
    bind(fd, (struct sockaddr*)&client, sizeof(client));

    // 03. 加入到组播地址
    struct ip_mreqn fl;
    inet_pton(AF_INET, "239.0.0.10", &fl.imr_multiaddr.s_addr);
    inet_pton(AF_INET, "0.0.0.0", &fl.imr_address.s_addr);
    fl.imr_ifindex = if_nametoindex("enp9s0f2");
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &fl, sizeof(fl));

    struct sockaddr_in from;
    socklen_t cli_len = sizeof(from);

    while(1)
    {
        // 04. 接受组播消息
        char buf[255] = {0};
        
        int ret = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&from, &cli_len);
        if(ret == -1)
        {
            perror("send error");
            exit(1);
        }
        else
        {
            // 05. 打印信息
            char ip[64] = {0};
            printf("Recv Server IP: %s, Port: %d, Data: %s \n", 
                    inet_ntop(AF_INET, &from.sin_addr.s_addr, ip, sizeof(ip)),
                    ntohs(from.sin_port),
                    buf);
        }
    }

    close(fd);
    return 0;
}

//组播客户端使用2
/*
m_commFd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);

if(m_commFd < 0)
{
    printf("[ERROR]: %s creat socket failed \n", m_connName.c_str());
}
else
{
    //set for reuse to allow faster reconnect
    setSocetReuseAddr(m_commFd);

    //bind using multicast ip and port
    if(bind(m_commFd, (struct sockaddr*)&m_remoteAddr, sizeof(m_remoteAddr)) != 0)
    {
        printf("[ERROR]: %s bind socket failed \n", m_connName.c_str());
        close(m_commFd);
        m_commFd = -1;
    }
    else
    {
        //add to multicast group, quest multicast group deliverd to the local interface
        struct ip_mreq group;
        group.imr_multiaddr.s_addr = m_remoteAddr.sin_addr.s_addr;
        group.imr_interface.s_addr = m_localAddr.sin_addr.s_addr;

        errno = 0;
        if(setsockopt(m_commFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) < 0)
        {
            printf("[ERROR]: %s set socket option failed \n", m_connName.c_str());
            //clean up for retry
        }

        else
        {
            printf("[INFO]: %s ready for communication \n", m_connName.c_str());
            m_connState = ConnState::online;
        }
    }
}
*/


