#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#define MAX 100
#define PORT 9090

int main()
{
	int s_sock,n;
	char buff[255];
	struct sockaddr_in server,client;
	socklen_t client_len;
	s_sock = socket(AF_INET,SOCK_DGRAM,0);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	if(bind(s_sock,(struct sockaddr*)&server,sizeof(server))==-1)
	{
		printf("Binding Failure\n");
		return 0;

	}

	else
	{
		client_len = sizeof(client);

		while(1)
		{
			bzero(buff,255);
			n=recvfrom(s_sock,buff,sizeof(buff),0,(struct sockaddr*)&client,&client_len);
			printf("Message from client : %s \n",buff);
			bzero(buff,255);
			printf("Enter a message for the client : ");

			fgets(buff,255,stdin);
			n = sendto(s_sock,buff,sizeof(buff),0,(struct sockaddr*)&client,client_len);
		}
		close(s_sock);
		return 0;

	}
	

}