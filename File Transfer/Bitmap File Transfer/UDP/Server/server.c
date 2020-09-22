// DEPENDENCIES
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>

// Declare a funtion to print print_error message 
void print_error(const char* msg)
{
	perror(msg);
	exit(1);
}


int main()
{	
	// Declare server socket
	int s_sock;
	
	// Create server socket 
	s_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (s_sock < 0)
		print_error("[-] SOCKET CREATION FAILURE\n");

	// Declarations
	struct sockaddr_in server, client;
	socklen_t client_len;
	
	// Ensure that adequate memory is provided to server and client
	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));


	// Define the server 
	server.sin_family = AF_INET;
	server.sin_port = htons(9090);
	server.sin_addr.s_addr = INADDR_ANY;

	client_len = sizeof(client);
	
	if(bind(s_sock, (struct sockaddr*)&server, sizeof(server)) != -1)
	{
		printf("SOCKET CREATION AND BINDING SUCCESSFUL \n");
		printf("This is the server speaking\n\n");
	}
	else
	{
		print_error("[-] BINDING FAILURE\n");
		return 1;
	} 

	char filename[100], recieved_file[100] = "", ext[10], buffer[2000], buf[1];

	//Recieve the filename and the extension from the client
	recvfrom(s_sock, filename, sizeof(filename), 0, (struct sockaddr*)&client, &client_len);
	recvfrom(s_sock, ext, sizeof(ext), 0, (struct sockaddr*)&client, &client_len);

	//Create a new file to dump the content into the file
	strcat(recieved_file, filename);
	strcat(recieved_file, "_recieved.");
	strcat(recieved_file, ext);

	//Open the desired file in write bytes mode to ensure that a bit stream flows
	FILE *fp;
	fp = fopen(recieved_file, "wb");

	// Declare end of file
	char eof[1] = "0";

	int flag = 0;
	while(flag == 0)
	{
		//Recieve the data from the client till the end of the file is not recieved
		recvfrom(s_sock, eof, sizeof(eof), 0, (struct sockaddr*)&client, &client_len);
		
		if(eof[0] == '3')
			flag = 1;

		else
		{
			
			recvfrom(s_sock, buf, sizeof(buf), 0, (struct sockaddr*)&client, &client_len);
			// Recieve bit by bit and dump the data into the new file
			fprintf(fp, "%c", buf[0]);
		}
	}
	
	printf("\n\n[+] COMPLETED\n\n");

	//Close the file
	fclose(fp);

	printf("[+] The following file has been recieved successfully : %s .\n", recieved_file);

	//Close the server
	printf("[+] CLOSING SERVER");
	close(s_sock);

	return 0;
}

