#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_PORT 8888

int main(int argc, char *argv[])
{
	
	char buf[256];

   //01. creat socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  //02. connect to server
  struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr);
	servaddr.sin_port = htons(SERV_PORT);

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  //03. communication with Server
	while (fgets(buf, sizeof(buf), stdin) != NULL) 
  {
		send(sockfd, buf, strlen(buf), 0);
		int recv_len = recv(sockfd, buf, sizeof(buf), 0);
		if (recv_len == 0)
		    printf("the other side has been closed.\n");
        break;
		else
			  printf("recv data: %s \n", buf);
	}

	close(sockfd);

	return 0;
}
