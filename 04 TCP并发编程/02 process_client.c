#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define PORT 8888



int main(int argc, char * argv[])
{
    struct sockaddr_in server_addr;

    //01. creat socket
    int cfd = socket(AF_INET, SOCK_STREAM, 0);

    //02. connect

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "10.93.9.148", &server_addr.sin_addr); //all local ip
    server_addr.sin_port = htons(PORT);

    connect(cfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while(1)
    {
        printf("please input data: \n");
        char buf[255] = {0};

        fgets(buf, sizeof(buf), stdin);

        //03 send data
        send(cfd, buf, sizeof(buf), 0);

        //04 read data
        printf("wait reply from server ...\n");
        int r_len = recv(cfd, buf, sizeof(buf), 0);
        if(r_len == 0)
        {
            printf("server closed \n");
            break;
        }
        else
        {
            printf("server reply:%s \n", buf);
        }
    }
    close(cfd);

    return 0;
}

