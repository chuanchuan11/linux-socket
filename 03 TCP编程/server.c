#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

#define PORT 8888
#define IP "127.0.0.1"

int main()
{
    //1. creat socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("creat socket failed");
    }
    else
    {
        struct sockaddr_in server_addr;


        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
#if 0
        inet_pton(AF_INET, IP, &server_addr.sin_addr)
#else
        //use INADDR_ANY
        server_addr.sin_addr.s_addr = INADDR_ANY;

#endif
        
        //2. bind server ip and port
        if(bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            perror("bind error");
        }
        else
        {
            //3. listen client request
            if(listen(lfd, 5) == -1)
            {
                perror("listen error");
            }
            else
            {
                printf("wait clinet request connect \n");
                struct sockaddr_in client_info;
                socklen_t client_len = sizeof(client_info);
                //4. wait client connect
                int cfd = accept(lfd, (struct sockaddr *)&client_info, &client_len);
                if(cfd < 0)
                {
                    perror("connect error");
                }
                else
                {
                    printf("connect with: %s:%d \n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

                    //5. read data
                    while(1)
                    {
                        char recv_buffer[255];
                        bzero(recv_buffer, sizeof(recv_buffer));

                        int recv_len = recv(cfd, &recv_buffer, sizeof(recv_buffer), 0);
                        if(recv_len < 0)
                        {
                            perror("receive failed");
                        }
                        else if(recv_len == 0)
                        {
                            printf("client disconnect \n");
                            break;
                        }
                        else
                        {
                            printf("receive len: %d, data : %s \n", recv_len, recv_buffer);

                            //lower case -> upper case
                            for(int num = 0; num < recv_len; num ++)
                            {
                                recv_buffer[num] = toupper(recv_buffer[num]);
                            }

                            int send_len = send(cfd, recv_buffer, recv_len, 0);
                            if(send_len == -1)
                            {
                                perror("send failed");
                            }
                            else
                            {
                                printf("sned to client len: %d, data : %s \n", send_len, recv_buffer);
                            }
                        }
                    }

                    close(lfd);
                    close(cfd);
                    printf("close connection success \n");
                }
            }
        }
        
    }

    return 0;
} 




