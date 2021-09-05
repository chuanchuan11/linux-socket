#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8888
#define IP "127.0.0.1"


int main()
{
    //1. creat socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("client creat socket failed");
    }
    else
    {
        struct sockaddr_in server_addr;
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        inet_pton(AF_INET, IP, &server_addr.sin_addr);

        //2. connect server
        int result = connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

        if(result == -1)
        {
            perror("conncet to server failed");
        }
        else
        {
            printf("connect to server success \n");
            //3. communication with server
            while(1)
            {
                char send_buffer[255];
                char recv_buffer[255];
                bzero(send_buffer, sizeof(send_buffer));

                printf("please input data that you want to send to server: \n");
                fgets(send_buffer, sizeof(send_buffer), stdin);

                int send_len = send(fd, send_buffer, strlen(send_buffer), 0);
                if(send_len == -1)
                {
                    perror("send to server failed");
                }
                else
                {
                    printf("send successfully \n");
                    bzero(recv_buffer, sizeof(recv_buffer));
                    int recv_len = recv(fd, recv_buffer, sizeof(recv_buffer), 0);

                    if(recv_len == -1)
                    {
                        perror("receive server reply failed");
                    }
                    else
                    {
                        printf("receive server reply, len: %d, data: %s \n", recv_len, recv_buffer);
                    }
                }
            }
        }
    }


    return 0;
} 
