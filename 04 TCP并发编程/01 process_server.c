#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define PORT 8888

void do_sigchild(int num)
{
    while(waitpid(0, NULL, WNOHANG) > 0);
}

int main()
{
    // use sigchld to wait child process end
    sigset_t myset;
    sigemptyset(&myset);
    sigaddset(&myset, SIGCHLD);
    // mask the signal
    sigprocmask(SIG_BLOCK, &myset, NULL);

    //01. creat socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    //02. set addr reuse fastly
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //03. bind
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //all local ip
    server_addr.sin_port = htons(PORT);

    bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    //04. listen
    listen(lfd, 20);

    while(1)
    {
        printf("wait client conncting...... \n");

        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        int cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_len);

        //05. creat new child process when accept the client conncet
        pid_t pid = fork();

        if(0 == pid)    //child process
        {
            // 01. close lfd
            close(lfd);
            while(1)
            {
                printf("wait client message ... \n");
                char buf[255] = {0};

                // 02. receice data from client
                int recv_len = recv(cfd, buf, sizeof(buf), 0);
                if(recv_len == 0)
                {
                    printf("client disconnect \n");
                    break;
                }
                else
                {
                    printf("received from [ip:%s, port:%d] data: %s, recv_len:%d \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf, recv_len);

                    for(int index = 0; index < recv_len; index++)
                    {
                        buf[index] = toupper(buf[index]);
                    }

                    int send_len = send(cfd, buf, recv_len, 0);
                    if(send_len == -1)
                    {
                        perror("send failed");
                    }
                    else
                    {
                        printf("send to client len: %d, data : %s \n", send_len, buf);
                    }
                }
            }
            // 03 close connection fd, and end child process
            close(cfd);
            return 0;
        }
        else if(pid > 0) //father process
        {
            struct sigaction act;
            act.sa_flags = 0;
            act.sa_handler = do_sigchild;
            sigemptyset(&act.sa_mask);
            sigaction(SIGCHLD, &act, NULL);

            //remove the mask of sigchld
            sigprocmask(SIG_UNBLOCK, &myset, NULL);

            close(cfd);
        }
        else //creat child process failed
        {
            perror("creat child process failed");
        }
    }

    return 0;
}

