#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8888

struct s_info
{
    struct sockaddr_in clientaddr;
    int confd;
};

void * work(void * arg)
{
    struct s_info *ts = (struct s_info*)arg;

    while (1) 
    {
        char buf[256] = {0};
        char ip[32] = {0};
        int recv_len = recv(ts->confd, buf, sizeof(buf), 0);      //读客户端
        if (recv_len == 0) 
        {
            printf("the client %d closed...\n", ts->confd);
            break;                                              //跳出循环,关闭cfd
        }
        printf("received from [ip:%s  PORT:%d], data:%s \n",
                inet_ntop(AF_INET, &(*ts).clientaddr.sin_addr, ip, sizeof(ip)),
                ntohs((*ts).clientaddr.sin_port), buf);

        for (int i = 0; i < recv_len; i++) 
        {
            buf[i] = toupper(buf[i]);
        }

        send(ts->confd, buf, recv_len, 0);
    }
    close(ts->confd);

    return NULL;
}

int main()
{
    struct s_info ts[256];  //根据最大线程数创建结构体
    int i = 0;  //已经连接的server端个数
    pthread_t tid;

    //01 . 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    //02 . bind到server端
    struct sockaddr_in server_addr;
    bzero(&server_addr, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    //03. listen
    listen(lfd, 128);

    printf("wait clinet connect request... \n");
    while(1)
    {
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);

        //04 . accept connect from client
        int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_len);

        ts[i].clientaddr = client_addr;
        ts[i].confd = cfd;

        pthread_create(&tid, NULL, work, (void *)&ts[i]);

        pthread_detach(tid);  //分离子线程，防止僵尸进程的产生

        i++;
        if(i == 256)
        {
            break;
        }
    }
}


